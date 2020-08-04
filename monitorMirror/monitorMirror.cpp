// monitorMirror.cpp : Defines the entry point for the console application.
//

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <Windows.h>
using namespace cv;
using namespace std;

//Function heavilly inspired by https://raw.githubusercontent.com/sturkmen72/opencv_samples/master/Screen-Capturing.cpp
Mat getDesktopMat()
{
	Mat returnImg;
	HDC hwindowDC, hwindowCompatibleDC;
	int height, width, srcheight, srcwidth;
	HBITMAP hbwindow;
	BITMAPINFOHEADER  bi;
	HWND hwnd = GetDesktopWindow();
	hwindowDC = GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT windowsize;    // get the height and width of the screen
	GetClientRect(hwnd, &windowsize);

	srcheight = windowsize.bottom;
	srcwidth = windowsize.right;
	height = (int)(windowsize.bottom);
	width = (int)(windowsize.right);

	returnImg.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, returnImg.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

	return returnImg;
}

int main(int argc, char** argv)
{

	Mat image;
	namedWindow("Display window", WINDOW_NORMAL); // Create a window for display.
	while (true)
	{
		image = getDesktopMat();
		
		if (image.empty())
		{
			cout << "no image to display" << endl;
		}
		else
		{
			imshow("Display window", image); // Show our image inside it.
		}
		int key = waitKey(5);

		if (key == 27)//press esc to break loop
		{
			break;
		}

	}

	return 0;
}