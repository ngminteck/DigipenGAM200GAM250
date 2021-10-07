#include "AIScript.h"
namespace AIScript
{
	void MoveTowards_X(TransformComponent * transComp1, TransformComponent * transComp2, float speed)
	{
		Vector2D tmp = Vector2D(transComp1->GetPosition().x - transComp2->GetPosition().x, 0.0f);
		Vector2DNormalize(tmp);
		transComp1->SetPosition(transComp1->GetPosition() + tmp*speed * 1/60);
	}

}