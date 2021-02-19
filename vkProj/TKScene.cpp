//
//TKScene.cpp
//

#include "TKScene.h"
#include "TKPrefix.h"

using namespace TK;

Scene::Scene() {}

Scene::~Scene() {}

Scene *Scene::create(const VkInstance &inst, const VkSurfaceKHR &surface) {
	Scene *s = new Scene();
	if(s->init(inst, surface)==false){
		delete s;
		return nullptr;
	}
	return s;
}

bool Scene::init(const VkInstance &inst, const VkSurfaceKHR &surface) {
	m_instance = inst;
	m_surface = surface;

	uint32_t width, height;
	uint32_t physicalDeviceCount;
	vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, nullptr);
	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, physicalDevices.data());
	for(uint32_t i=0; i<physicalDeviceCount; ++i){
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(physicalDevices[i], &props);
		if(props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
			TKlog("device[%d] %s", i, props.deviceName);
		}
	}
	pDevice = Device::create(m_instance, physicalDevices[0]);
	if(pDevice == nullptr){
		TKlog("device create error!\n");
		return false;
	}
	pSwapchain = Swapchain::create(physicalDevices[0], m_surface);
	pSwapchain->setupSwapchain(&width, &height, pDevice->logicalDevice());
	m_width = width;
	m_height = height;

	this->setupDepthImageBuffer();
	TKlog("setup depth stencil buffer");

	TKlog("init render pass");
    pRenderPass = RenderPass::create(pDevice->logicalDevice(),
									 pSwapchain->colorFormat(),
									 pDevice->depthFormat());

	pBasicPipeline = Pipeline::create(pDevice->logicalDevice(),
									  pRenderPass->renderPass());
	
	TKlog("init command pool");
    m_cmdPool = pDevice->createCommandPool(pSwapchain->queueFamilyIndex());
	m_drawCmdBuffers.resize(pSwapchain->imageCount());
	for(uint i=0; i<pSwapchain->imageCount(); ++i){
		m_drawCmdBuffers[i] = pDevice->createCommandBuffer(m_cmdPool);
	}
	
	this->setupFrameBuffers();
	this->setupDescriptorPool();
	this->prepareVertices();
	this->setupUniformBuffers();
	this->setupDescriptorSet();
	this->prepareSynchronizationPrimitives(pDevice->logicalDevice());
	this->buildCommandBuffers();
	return true;
}

void Scene::prepareSynchronizationPrimitives(const VkDevice &device) {
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = nullptr;
	vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &m_presentSemaphore);
	vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &m_renderSemaphore);

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.pNext = nullptr;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	m_waitFences.resize(m_drawCmdBuffers.size());
	for(auto &fence : m_waitFences){
		vkCreateFence(device, &fenceCreateInfo, nullptr, &fence);	
	}
}

void Scene::destroy() {
	if(m_renderSemaphore != VK_NULL_HANDLE){
		vkDestroySemaphore(pDevice->logicalDevice(), m_renderSemaphore, nullptr);
	}
	if(m_presentSemaphore != VK_NULL_HANDLE){
		vkDestroySemaphore(pDevice->logicalDevice(), m_presentSemaphore, nullptr);
	}
	for(auto &fence : m_waitFences){
		vkDestroyFence(pDevice->logicalDevice(), fence, nullptr);
	}
	if(m_cmdPool != VK_NULL_HANDLE){
		vkDestroyCommandPool(pDevice->logicalDevice(), m_cmdPool, nullptr);
	}

	if(m_depthImage != VK_NULL_HANDLE){
		vkDestroyImage(pDevice->logicalDevice(), m_depthImage, nullptr);
	}
   
	vkFreeMemory(pDevice->logicalDevice(), m_depthImageMem, nullptr);
	if(m_depthImageView != VK_NULL_HANDLE){
		vkDestroyImageView(pDevice->logicalDevice(), m_depthImageView, nullptr);
	}
	for(uint32_t i=0; i<pSwapchain->imageCount(); ++i){
		vkDestroyFramebuffer(pDevice->logicalDevice(), m_framebuffers[i], nullptr);
	}

	if(m_descriptorPool != VK_NULL_HANDLE){
		vkDestroyDescriptorPool(pDevice->logicalDevice(), m_descriptorPool, nullptr);
	}

	if(m_verticeBuf.buffer != VK_NULL_HANDLE){
		vkFreeMemory(pDevice->logicalDevice(), m_verticeBuf.memory, nullptr);
		vkDestroyBuffer(pDevice->logicalDevice(), m_verticeBuf.buffer, nullptr);
	}

	if(m_indiceBuf.buffer != VK_NULL_HANDLE){
		vkFreeMemory(pDevice->logicalDevice(), m_indiceBuf.memory, nullptr);
		vkDestroyBuffer(pDevice->logicalDevice(), m_indiceBuf.buffer, nullptr);
	}
	
	pBasicPipeline->destroy();	
	pRenderPass->destroy();
	pSwapchain->clean();
}

void Scene::setSize(uint32_t w, uint32_t h) {
	m_width  = w;
	m_height = h;
}

void Scene::setupFrameBuffers() {
	m_framebuffers.resize(pSwapchain->imageCount());
	for(uint32_t i=0; i<pSwapchain->imageCount(); ++i){ 
		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.pNext = nullptr;
		framebufferCreateInfo.width = m_width;
		framebufferCreateInfo.height = m_height;
		framebufferCreateInfo.renderPass = pRenderPass->renderPass();
		framebufferCreateInfo.layers = 1;
		std::array<VkImageView, 2> attachments;
		attachments[0] = pSwapchain->imageView(i);
		attachments[1] = m_depthImageView;
		framebufferCreateInfo.attachmentCount = attachments.size();
		framebufferCreateInfo.pAttachments = attachments.data();
        vkCreateFramebuffer(pDevice->logicalDevice(), &framebufferCreateInfo, nullptr, &m_framebuffers[i]);
	}
}

void Scene::setupDepthImageBuffer() {
	VkImageCreateInfo depthImageInfo = {};
	depthImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	depthImageInfo.format = pDevice->depthFormat();
	depthImageInfo.imageType = VK_IMAGE_TYPE_2D;
	depthImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	depthImageInfo.extent = {m_width, m_height, 1};
	depthImageInfo.mipLevels = 1;
	depthImageInfo.arrayLayers = 1;
	depthImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	depthImageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	depthImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	vkCreateImage(pDevice->logicalDevice(), &depthImageInfo, nullptr, &m_depthImage);
	VkMemoryAllocateInfo memAlloc = {};
	memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAlloc.pNext = nullptr;
	VkMemoryRequirements memReqs;
	vkGetImageMemoryRequirements(pDevice->logicalDevice(), m_depthImage, &memReqs);
	memAlloc.allocationSize = memReqs.size;
	memAlloc.memoryTypeIndex =
		pDevice->getMemoryTypeIndex(memReqs.memoryTypeBits,
									VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	vkAllocateMemory(pDevice->logicalDevice(), &memAlloc, nullptr, &m_depthImageMem);
    vkBindImageMemory(pDevice->logicalDevice(), m_depthImage, m_depthImageMem, 0);

    VkImageViewCreateInfo depthImageViewInfo = {};
    depthImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    depthImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthImageViewInfo.format = pDevice->depthFormat();
    depthImageViewInfo.image = m_depthImage;
    depthImageViewInfo.subresourceRange.aspectMask =
        VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    depthImageViewInfo.subresourceRange.baseArrayLayer = 0;
    depthImageViewInfo.subresourceRange.baseMipLevel = 0;
    depthImageViewInfo.subresourceRange.layerCount = 1;
    depthImageViewInfo.subresourceRange.levelCount = 1;
    vkCreateImageView(pDevice->logicalDevice(), &depthImageViewInfo,
					  nullptr, &m_depthImageView);
}

void Scene::prepareVertices() {
	void *data;
	VkDevice device = pDevice->logicalDevice();
	static Vertex vertexBufferData[] =
	{{ { 0.5f,  0.5f, 0.0f},  {0.8f, 0.0f, 0.0f} },
	 { {-0.5f,  0.5f, 0.0f},  {0.0f, 0.8f, 0.0f} },
	 { {-0.5f, -0.5f, 0.0f},  {0.0f, 0.0f, 0.8f} },
	 { { 0.5f, -0.5f, 0.0f},  {0.0f, 0.8f, 0.0f} }};
	
	uint32_t vertexBufferSize = sizeof(Vertex)*4;
	static uint32_t indexBufferData[] = { 0, 1, 2, 0, 2, 3 };
	uint32_t indexBufferSize = sizeof(uint32_t)*6;
   
	m_verticeBuf.buffer = VK_NULL_HANDLE;
	
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferCreateInfo.size = vertexBufferSize;
	bufferCreateInfo.queueFamilyIndexCount = 0;
	bufferCreateInfo.pQueueFamilyIndices = nullptr;
	vkCreateBuffer(device, &bufferCreateInfo, nullptr, &(m_verticeBuf.buffer));
	vkGetBufferMemoryRequirements(device, m_verticeBuf.buffer, &m_verticeBuf.memoryReq);
	
	VkMemoryAllocateInfo vertexMemAlloc = {};
	vertexMemAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vertexMemAlloc.allocationSize = m_verticeBuf.memoryReq.size;
	vertexMemAlloc.memoryTypeIndex =
		pDevice->getMemoryTypeIndex(m_verticeBuf.memoryReq.memoryTypeBits,
									VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	if(vertexMemAlloc.memoryTypeIndex == UINT32_MAX){
		TKerror("invalid vertex buffer memory type index %d", vertexMemAlloc.memoryTypeIndex);
	}else{
		vkAllocateMemory(device, &vertexMemAlloc, nullptr, &m_verticeBuf.memory);
		vkMapMemory(device, m_verticeBuf.memory, 0, vertexMemAlloc.allocationSize, 0, &data);
		memcpy(data, vertexBufferData, vertexBufferSize);
		vkUnmapMemory(device, m_verticeBuf.memory);
		vkBindBufferMemory(device, m_verticeBuf.buffer, m_verticeBuf.memory, 0);
	}
	
	VkBufferCreateInfo indiceBufferCreateInfo = {};
	indiceBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	indiceBufferCreateInfo.queueFamilyIndexCount = 0;
	indiceBufferCreateInfo.pQueueFamilyIndices = nullptr;
	indiceBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	indiceBufferCreateInfo.size = indexBufferSize;
	indiceBufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	vkCreateBuffer(device, &indiceBufferCreateInfo, nullptr, &m_indiceBuf.buffer);
	vkGetBufferMemoryRequirements(device, m_indiceBuf.buffer, &m_indiceBuf.memoryReq);
	
	VkMemoryAllocateInfo indiceMemAlloc = {};
	indiceMemAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	indiceMemAlloc.allocationSize = m_indiceBuf.memoryReq.size;
	indiceMemAlloc.memoryTypeIndex =
		pDevice->getMemoryTypeIndex(m_indiceBuf.memoryReq.memoryTypeBits,
									VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
									VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	
	if(indiceMemAlloc.memoryTypeIndex == UINT32_MAX){
		TKerror("invalid indice buffer memroy type index %d", indiceMemAlloc.memoryTypeIndex);
	}else{
		vkAllocateMemory(device, &indiceMemAlloc, nullptr, &m_indiceBuf.memory);
		vkMapMemory(device, m_indiceBuf.memory, 0, indiceMemAlloc.allocationSize, 0, &data);
		memcpy(data, indexBufferData, indexBufferSize);
		vkUnmapMemory(device, m_indiceBuf.memory);
		vkBindBufferMemory(device, m_indiceBuf.buffer, m_indiceBuf.memory, 0);
	}
}

void Scene::setupUniformBuffers() {
	VkBufferCreateInfo bufferCreateInfo= {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.pQueueFamilyIndices = nullptr;
	bufferCreateInfo.queueFamilyIndexCount = 0;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	bufferCreateInfo.size = sizeof(float)*32;
	vkCreateBuffer(pDevice->logicalDevice(), &bufferCreateInfo, nullptr, &m_uniformBuf.buffer);
	vkGetBufferMemoryRequirements(pDevice->logicalDevice(), m_uniformBuf.buffer, &m_uniformBuf.memoryReq);

	VkMemoryAllocateInfo memAllocInfo = {};
	memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllocInfo.allocationSize = m_uniformBuf.memoryReq.size;
	memAllocInfo.memoryTypeIndex =
		pDevice->getMemoryTypeIndex(m_uniformBuf.memoryReq.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	vkAllocateMemory(pDevice->logicalDevice(), &memAllocInfo, nullptr, &m_uniformBuf.memory);
	vkBindBufferMemory(pDevice->logicalDevice(), m_uniformBuf.buffer, m_uniformBuf.memory, 0);	
}

void Scene::setupDescriptorPool() {
	VkDescriptorPoolSize typeCounts[1];
	typeCounts[0].descriptorCount = 1;
	typeCounts[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	
	VkDescriptorPoolCreateInfo poolCreateInfo = {};
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolCreateInfo.pNext = nullptr;
	poolCreateInfo.maxSets = 1;
	poolCreateInfo.poolSizeCount = 1;
	poolCreateInfo.pPoolSizes = typeCounts;
	vkCreateDescriptorPool(pDevice->logicalDevice(), &poolCreateInfo, nullptr, &m_descriptorPool);
}

void Scene::setupDescriptorSet() {
	VkDescriptorSetLayout descSetLayout = pBasicPipeline->descriptorSetLayout();
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &descSetLayout;
	vkAllocateDescriptorSets(pDevice->logicalDevice(), &allocInfo, &m_descriptorSet);

	VkWriteDescriptorSet writeDescSet = {};
	writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescSet.dstSet = m_descriptorSet;
	writeDescSet.descriptorCount = 1;
	writeDescSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	VkDescriptorBufferInfo descBufferInfo = {};
	descBufferInfo.buffer = m_uniformBuf.buffer;
	descBufferInfo.offset = 0;
	descBufferInfo.range = sizeof(float)*32;
	writeDescSet.pBufferInfo = &descBufferInfo;
}

void Scene::buildCommandBuffers() {
	TKlog("build command buffers");
	VkCommandBufferBeginInfo cmdBufBeginInfo = {};
	cmdBufBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBufBeginInfo.pNext = nullptr;

	VkClearValue clearValues[2];
	clearValues[0].color = {{0.707f, 0.707f, 0.707f, 1.0f}};
	clearValues[1].depthStencil = {1.0f, 0};

	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.pNext = nullptr;
	renderPassBeginInfo.clearValueCount = 2;
	renderPassBeginInfo.pClearValues = clearValues;
	renderPassBeginInfo.renderPass = pRenderPass->renderPass();
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent.width = m_width;
	renderPassBeginInfo.renderArea.extent.height = m_height;
	for(uint32_t i=0; i<m_drawCmdBuffers.size(); ++i){
		renderPassBeginInfo.framebuffer = m_framebuffers[i];
		
		vkResetCommandBuffer(m_drawCmdBuffers[i], VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
		vkBeginCommandBuffer(m_drawCmdBuffers[i], &cmdBufBeginInfo);
		vkCmdBeginRenderPass(m_drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport = {};
		viewport.width = (float)m_width;
		viewport.height = (float)m_height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		viewport.x = 0;
		viewport.y = 0;
		vkCmdSetViewport(m_drawCmdBuffers[i], 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		scissor.extent.width = m_width;
		scissor.extent.height = m_height;
		vkCmdSetScissor(m_drawCmdBuffers[i], 0, 1, &scissor);

		
		//vkCmdBindDescriptorSets(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
		//						pBasicPipeline->pipelineLayout(), 0, 1, &m_descriptorSet, 0, nullptr);
		
		vkCmdBindPipeline(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pBasicPipeline->pipeline());
		VkDeviceSize offsets[1] = {0};

		vkCmdBindVertexBuffers(m_drawCmdBuffers[i], 0, 1, &m_verticeBuf.buffer, offsets);
		vkCmdBindIndexBuffer(m_drawCmdBuffers[i], m_indiceBuf.buffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(m_drawCmdBuffers[i], 6, 1, 0, 0, 0);

		vkCmdEndRenderPass(m_drawCmdBuffers[i]);
		vkEndCommandBuffer(m_drawCmdBuffers[i]);
	}
}

void Scene::draw() {
	vkAcquireNextImageKHR(pDevice->logicalDevice(), pSwapchain->swapchain(),
						  UINT64_MAX, m_presentSemaphore, nullptr, &m_currentIndex);
	//this->buildCommandBuffers(m_currentIndex);
	VkResult ret;
	do{
		ret = vkWaitForFences(pDevice->logicalDevice(), 1, &m_waitFences[m_currentIndex], VK_TRUE, UINT64_MAX);
	}while(ret == VK_TIMEOUT);
  
	vkResetFences(pDevice->logicalDevice(), 1, &m_waitFences[m_currentIndex]);
	
	VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pWaitDstStageMask = &waitStageMask;
	submitInfo.pWaitSemaphores = &m_presentSemaphore;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &m_renderSemaphore;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_drawCmdBuffers[m_currentIndex];
	vkQueueSubmit(pDevice->graphicsQueue(), 1, &submitInfo, m_waitFences[m_currentIndex]);
	
	VkSwapchainKHR swapchain = pSwapchain->swapchain();
    VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pImageIndices = &m_currentIndex;
	presentInfo.pSwapchains = &swapchain;
	presentInfo.swapchainCount = 1;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &m_renderSemaphore;
	ret = vkQueuePresentKHR(pDevice->graphicsQueue(), &presentInfo);
	if(ret != VK_SUCCESS || ret == VK_SUBOPTIMAL_KHR){
		TKerror("present error!");
	}
}


