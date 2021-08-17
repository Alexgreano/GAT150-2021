#include "Transform.h"

namespace nc 
{
	void Transform::Update()
	{
		Matrix33 mxs;
		mxs.Scale(scale);

		Matrix33 mxr;
		mxr.Rotate(rotation);

		Matrix33 mxt;
		mxt.Translate(position);

		matrix = mxs * mxr * mxt;

	}
	void Transform::Update(const Matrix33& mx)
	{
		Matrix33 mxs;
		mxs.Scale(localscale);

		Matrix33 mxr;
		mxr.Rotate(localrotation);

		Matrix33 mxt;
		mxt.Translate(localposition);

		matrix = mxs * mxr * mxt * mx;

		position = matrix.GetTranslate();
		rotation = matrix.GetRotation();
		scale = matrix.GetScale();
	}
}