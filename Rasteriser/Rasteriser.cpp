#include "Rasteriser.h"
#include <time.h>


Rasteriser app;

float rotationAngle = float(M_PI_2);
float scale = 1;
float translateX = 0;
float translateY = 0;
float translateZ = 0;
const int timeFactor = 7000;

string Rasteriser::ModelPath()
{
	char buf[256];
	GetCurrentDirectoryA(256, buf);
	return std::string(buf) + '\\';
}

bool Rasteriser::Initialise()
{
	_modelpath="";
	_modelpath = ModelPath() + "\\cube.md2";

	if (!MD2Loader::LoadModel(_modelpath.c_str(), _model,
		&Model::AddPolygon,
		&Model::AddVertex))
	{
		return false;
	}
	_model;

	Camera temp(0, 0, 0, Vertex(0, 0, -100, 1)); //Camera temp(0, 0, 0, Vertex(0, 7, -11, 1));
	_camera = temp;
	_timeFactor = 7000;
	_rotationAngle = float(M_PI_2);

	return true;

	
}

void Rasteriser::DrawWireFrame(Bitmap &bitmap)
{
	std::vector<Polygon3D> tempPolygon(_model.GetPolygons());
	int polygonSize = int(_model.GetPolygonCount());
	std::vector<Vertex> tempVertex(_model.GetTransform());



	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HGDIOBJ oldPen = SelectObject(bitmap.GetDC(), hPen);
	for (int i = 0; i < polygonSize; i++)
	{
		if (!tempPolygon[i].GetCull())
		{
			//Get the indices
			int tempIndex = tempPolygon[i].GetIndex(0);
			Vertex tempVertex0(tempVertex[tempIndex]);
			int tempIndex1 = tempPolygon[i].GetIndex(1);
			Vertex tempVertex1(tempVertex[tempIndex1]);
			int tempIndex2 = tempPolygon[i].GetIndex(2);
			Vertex tempVertex2(tempVertex[tempIndex2]);

			//Draw the triangle
			MoveToEx(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY(), NULL);
			LineTo(bitmap.GetDC(), tempVertex1.GetIntX(), tempVertex1.GetIntY());
			LineTo(bitmap.GetDC(), tempVertex2.GetIntX(), tempVertex2.GetIntY());
			LineTo(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY());
		}
	}
	DeleteObject(hPen);
}

void Rasteriser::DrawSolidFlat(Bitmap& bitmap)
{
	std::vector<Vertex> tempVertex(_model.GetTransform());
	std::vector<Polygon3D> tempPolygon(_model.GetPolygons());
	int polygonSize = int(_model.GetPolygonCount());

	for (int i = 0; i < polygonSize; i++)
	{
		if (!tempPolygon[i].GetCull())
		{
			HPEN pen = CreatePen(PS_SOLID, 1, tempPolygon[i].GetColour());
			HGDIOBJ oldPen = SelectObject(bitmap.GetDC(), pen);

			HBRUSH brush = CreateSolidBrush(tempPolygon[i].GetColour());
			HGDIOBJ oldBrush = SelectObject(bitmap.GetDC(), brush);

			
			//Get the indices
			int tempIndex = tempPolygon[i].GetIndex(0);
			Vertex tempVertex0(tempVertex[tempIndex]);
			int tempIndex1 = tempPolygon[i].GetIndex(1);
			Vertex tempVertex1(tempVertex[tempIndex1]);
			int tempIndex2 = tempPolygon[i].GetIndex(2);
			Vertex tempVertex2(tempVertex[tempIndex2]);

			POINT temp0{ long(tempVertex0.GetX()), long(tempVertex0.GetY()) };
			POINT temp1{ long(tempVertex1.GetX()), long(tempVertex1.GetY()) };
			POINT temp2{ long(tempVertex2.GetX()), long(tempVertex2.GetY()) };

			POINT tempArray[] = { temp0, temp1, temp2 };

			Polygon(bitmap.GetDC(), tempArray, 3);

			DeleteObject(pen);
			DeleteObject(brush);
		}
	}
}

void Rasteriser::DrawString(Bitmap& bitmap, LPCTSTR text)
{
	HDC hdc = bitmap.GetDC();
	HFONT hFont, hOldFont;

	// Retrieve a handle to the variable stock font.  
	hFont = hFont = CreateFont(48, 18, 1, 30, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Myfont"));

	// Select the variable stock font into the specified device context. 
	if (hOldFont = (HFONT)SelectObject(hdc, hFont))
	{
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkColor(hdc, RGB(0, 0, 0));

		// Display the text string.  
		TextOut(hdc, 10, 10, text, lstrlen(text));

		// Restore the original font.        
		SelectObject(hdc, hOldFont);
	}
	DeleteObject(hFont);
}






void Rasteriser::GeneratePerspectiveMatrix(float d, float aspectRatio)
{
	Matrix perspectiveMatrix{ d / aspectRatio, 0, 0, 0,
										0, d, 0, 0,
										0, 0, d, 0,
										0, 0, 1, 0 };
	_perspectiveTransform = perspectiveMatrix;

}

void Rasteriser::GenerateViewMatrix(float d, int width, int height)
{
	Matrix viewMatrix{ float(width) / 2, 0, 0, float(width) / 2,
							0, float(-height) / 2, 0, float(height) / 2,
							0, 0, d / 2, d / 2,
							0, 0, 0, 1 };
	_screenTransform = viewMatrix;
}

void Rasteriser::Update(Bitmap &bitmap)
{
	if (clock() < 1 * timeFactor)
	{
		_rotationAngle += 0.02f;
		_modelTransform = Matrix::YRotationMatrix(_rotationAngle);
		DrawString(bitmap, L"Drawing the Wireframe with Backface Culling");
	}
	
	if (clock() > 1 * timeFactor && clock() <= 2 * timeFactor)
	{
		_rotationAngle += 0.02f;
		_modelTransform = Matrix::ZRotationMatrix(_rotationAngle);
		DrawString(bitmap, L"Rotation Around Z Axis");
	}
	if (clock() > 2 * timeFactor && clock() <= 4 * timeFactor)
	{
		_rotationAngle += 0.02f;
		_modelTransform = Matrix::XRotationMatrix(_rotationAngle);
		DrawString(bitmap, L"Rotation Around X Axis");
	}
	if (clock() > 4 * timeFactor && clock() <= 6 * timeFactor)
	{
		_rotationAngle += 0.02f;
		_modelTransform = Matrix::YRotationMatrix(_rotationAngle);
		DrawString(bitmap, L"Rotation Around Y Axis");
	}
	if (clock() > 6 * timeFactor && clock() <=	7 * timeFactor)
	{
		_rotationAngle += 0.02f;
		_scale += 0.008f;
		_modelTransform = Matrix::ScalingMatrix(_scale, _scale, _scale) * Matrix::YRotationMatrix(_rotationAngle) * Matrix::XRotationMatrix(_rotationAngle) * Matrix::ZRotationMatrix(_rotationAngle);
		DrawString(bitmap, L"Scaling in all Dimensions ");
	}
	if (clock() > 7 * timeFactor && clock() <= 8 * timeFactor)
	{
		_rotationAngle += 0.02f;
		_scale -= 0.008f;
		_modelTransform = Matrix::ScalingMatrix(_scale, _scale, _scale) * Matrix::YRotationMatrix(_rotationAngle) * Matrix::XRotationMatrix(_rotationAngle) * Matrix::ZRotationMatrix(_rotationAngle);
		DrawString(bitmap, L"Scaling in all Dimensions ");
	}
	if (clock() > 8 * timeFactor && clock() <= 8.5 * timeFactor)
	{
		translateX += 0.2f;
		_modelTransform = Matrix::TranslationMatrix(translateX, translateY, translateZ);
		DrawString(bitmap, L"Translation in all Axes ");
	}
	if (clock() > 8.5 * timeFactor && clock() <= 9 * timeFactor)
	{
		translateX = 0;
		translateZ += 0.2f;
		
		_modelTransform = Matrix::TranslationMatrix(translateX, translateY, translateZ);
		DrawString(bitmap, L"Translation in all Axes ");
	}
	if (clock() > 9 * timeFactor && clock() <= 9.5 * timeFactor)
	{
		translateX = 0;
		translateZ = 0;
		translateY += 0.2f;
		_modelTransform = Matrix::TranslationMatrix(translateX, translateY, translateZ);
		DrawString(bitmap, L"Translation in all Axes ");
	}

	if (clock() > 9.5 * timeFactor && clock() <= 11 * timeFactor)
	{
		_rotationAngle += 0.02f;
		_modelTransform = Matrix::YRotationMatrix(_rotationAngle);
		DrawString(bitmap, L"Faceted shading with GDI Polygon() ");
	}

	
	float aspectRatio = float(float(bitmap.GetWidth()) / float(bitmap.GetHeight()));
	GeneratePerspectiveMatrix(1, aspectRatio);
	GenerateViewMatrix(1, bitmap.GetWidth(), bitmap.GetHeight());
}

void Rasteriser::Render(Bitmap& bitmap)
{

	


	bitmap.Clear(RGB(0, 0, 0));
	_model.ApplyTransformToLocalVertices(_modelTransform);

	_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
	_model.CalculateBackfaces(_camera);
	_model.Sort();
	_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
	_model.Dehomogenize();
	_model.ApplyTransformToTransformedVertices(_screenTransform);

	if (clock() < 1 * timeFactor)
	{
		DrawWireFrame(bitmap);
		DrawString(bitmap, L"Drawing the Wireframe with Backface Culling");
	}
	if (clock() > 1 * timeFactor && clock() <= 2 * timeFactor)
	{
		DrawWireFrame(bitmap);
		DrawString(bitmap, L"Rotation Around Z Axis");
	}
	if (clock() > 2 * timeFactor && clock() <= 4 * timeFactor)
	{
		DrawWireFrame(bitmap);
		DrawString(bitmap, L"Rotation Around X Axis");
	}
	if (clock() > 4 * timeFactor && clock() <= 6 * timeFactor)
	{
		DrawWireFrame(bitmap);
		DrawString(bitmap, L"Rotation Around Y Axis");
	}
	if (clock() > 6 * timeFactor && clock() <= 8 * timeFactor)
	{
		DrawWireFrame(bitmap);
		DrawString(bitmap, L"Scaling in all Dimensions ");
	}
	if (clock() > 8 * timeFactor && clock() <= 9.5 * timeFactor)
	{
		DrawWireFrame(bitmap);
		DrawString(bitmap, L"Translation in all Axes ");
	}
	if (clock() > 9.5 * timeFactor && clock() <= 11 * timeFactor)
	{
		DrawSolidFlat(bitmap);
		DrawString(bitmap, L"Faceted shading with GDI Polygon() ");
	}

	


	

}

