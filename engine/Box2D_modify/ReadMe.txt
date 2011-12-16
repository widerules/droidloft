本人用使用ndk的版本为android_ndk_r5b

博客地址:http://blog.csdn.net/feifei454498130/article/details/6759298

编译前，请确认ndk环境已经配置好
请在些目录使用ndk-build命令编译即可

Box2D 版本为2.2
移植到android上，须要稍微修改一下源码。修改记录如下:
/***************************************************************************************************/
Box2D/Common/b2Math.h    注释#include <limits>
			//float32 infinity = std::numeric_limits<float32>::infinity(); 修改为以下
			float32 infinity = INFINITY;
/***************************************************************************************************/			
Box2D/Collision/b2BroadPhase.h   注释#include <algorithm>
				// 添加一个头文件  #include <stdlib.h>		
// 添加一个方法
static int b2PairQSORTLessThan (const void*element1, const void*element2)
{
	b2Pair *pair1 = (b2Pair*)element1;
	b2Pair *pair2 = (b2Pair*)element2;
	
	if (pair1->proxyIdA < pair2->proxyIdA)
		return -1;
	else if (pair1->proxyIdA > pair2->proxyIdA)
		return 1;
	else {
		if (pair1->proxyIdB < pair2->proxyIdB)
			return -1;
		else if (pair1->proxyIdB > pair2->proxyIdB)
			return 1;
	}
	return 0;
}

//std::sort(m_pairBuffer, m_pairBuffer + m_pairCount, b2PairLessThan);修改为以下
qsort (m_pairBuffer, m_pairCount, sizeof (b2Pair), b2PairQSORTLessThan);

/***************************************************************************************************/
Box2D/Dynamics/b2Body.h  
			注释#include <memory>  
			添加头文件 #include <memory.h>
/***************************************************************************************************/
Box2D/Common/b2BlockAllocator.cpp  //#include <memory> 注释掉
				   // 添加头文件 
				   #include <memory.h>
				  //#include <cstdlib> 注释掉
				  // 添加头文件 
			          #include <stdlib.h>

/***************************************************************************************************/
Box2D/Common/b2Settings.cpp
			//#include <cstdlib> 注释掉
			// 添加头文件
			#include <stdlib.h>

/***************************************************************************************************/
Box2D/Dynamics/b2Body.cpp
			// 添加头文件
			#include <new>
/***************************************************************************************************/

2011.9.8