#include "../TKPrefix.h"

namespace Model {

	class Base {
	public:
		Base();
		~Base();
		static Base *create();
		bool init();
	protected:
		VkBuffer m_vertBuf;
	};

	
};
