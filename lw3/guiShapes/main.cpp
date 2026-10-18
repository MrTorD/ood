#include "PictureDocument.h"
#include "EditorWindow.h"

int main()
{
	PictureDocument pictureDoc;

	EditorWindow window(pictureDoc);

	window.Run();
}