������ʹ��ndk�İ汾Ϊandroid_ndk_r5b

���͵�ַ:http://blog.csdn.net/feifei454498130/article/details/6759298

����ǰ����ȷ��ndk�����Ѿ����ú�
����ЩĿ¼ʹ��ndk-build������뼴��

Box2D �汾Ϊ2.2
��ֲ��android�ϣ���Ҫ��΢�޸�һ��Դ�롣�޸ļ�¼����:
/***************************************************************************************************/
Box2D/Common/b2Math.h    ע��#include <limits>
			//float32 infinity = std::numeric_limits<float32>::infinity(); �޸�Ϊ����
			float32 infinity = INFINITY;
/***************************************************************************************************/			
Box2D/Collision/b2BroadPhase.h   ע��#include <algorithm>
				// ���һ��ͷ�ļ�  #include <stdlib.h>		
// ���һ������
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

//std::sort(m_pairBuffer, m_pairBuffer + m_pairCount, b2PairLessThan);�޸�Ϊ����
qsort (m_pairBuffer, m_pairCount, sizeof (b2Pair), b2PairQSORTLessThan);

/***************************************************************************************************/
Box2D/Dynamics/b2Body.h  
			ע��#include <memory>  
			���ͷ�ļ� #include <memory.h>
/***************************************************************************************************/
Box2D/Common/b2BlockAllocator.cpp  //#include <memory> ע�͵�
				   // ���ͷ�ļ� 
				   #include <memory.h>
				  //#include <cstdlib> ע�͵�
				  // ���ͷ�ļ� 
			          #include <stdlib.h>

/***************************************************************************************************/
Box2D/Common/b2Settings.cpp
			//#include <cstdlib> ע�͵�
			// ���ͷ�ļ�
			#include <stdlib.h>

/***************************************************************************************************/
Box2D/Dynamics/b2Body.cpp
			// ���ͷ�ļ�
			#include <new>
/***************************************************************************************************/

2011.9.8