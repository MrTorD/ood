#include "EditorWindow.h"
#include "Ellipse.h"
#include "Picture.h"
#include "Triangle.h"

int main()
{
	Picture pic;

	EditorWindow window(pic);

	window.Run();
}