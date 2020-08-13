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

	HWND mirrorHwnd = (HWND)cvGetWindowHandle("Mirror window");
	
	//ShowWindow(mirrorHwnd, SW_MINIMIZE);
	HWND hwnd = GetDesktopWindow();
	//ShowWindow(mirrorHwnd, SW_SHOWDEFAULT);



	hwindowDC = GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT srcSize;
	GetClientRect(hwnd, &srcSize);

	RECT windowsize;    // get the height and width of the screen
	GetClientRect(hwnd, &windowsize);
	//windowsize.left = 400;
	//windowsize.bottom = 200;

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
	//StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	BitBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, SRCCOPY);
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, returnImg.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

	return returnImg;
	


	/*
	//Does the same as above but is apparently better
	HWND mirrorHwnd = (HWND)cvGetWindowHandle("Mirror window");
	RECT rc;
	ShowWindow(mirrorHwnd, SW_MINIMIZE);
	HWND hwnd = GetDesktopWindow();
	ShowWindow(mirrorHwnd, SW_RESTORE);
	GetClientRect(hwnd, &rc);
	int width = rc.right;
	int height = rc.bottom;

	Mat src;
	
	if (rc.right > 0)
	{
		
		src.create(height, width, CV_8UC4);
		HDC hdc = GetDC(hwnd);
		HDC memdc = CreateCompatibleDC(hdc);
		HBITMAP hbitmap = CreateCompatibleBitmap(hdc, width, height);
		HBITMAP oldbmp = (HBITMAP)SelectObject(memdc, hbitmap);

		BitBlt(memdc, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
		SelectObject(memdc, oldbmp);

		BITMAPINFOHEADER  bi = { sizeof(BITMAPINFOHEADER), width, -height, 1, 32, BI_RGB };
		GetDIBits(hdc, hbitmap, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

		DeleteObject(hbitmap);
		DeleteDC(memdc);
		ReleaseDC(hwnd, hdc);
	}
	return src;
	*/

}

int main(int argc, char** argv)
{
	const string windowName = "Mirror window";
	const int flipY = 1;
	Mat image;
	namedWindow(windowName, WINDOW_NORMAL); // Create a window for display.
	while (true)
	{
		image = getDesktopMat();
		
		if (image.empty())
		{
			cout << "no image to display" << endl;
		}
		else
		{
			//Cut the image to the size of the window
			Rect winRect = getWindowImageRect(windowName);
			Mat croppedImg = image(Rect(0, 0, winRect.width, winRect.height));


			//HWND hwnd = (HWND)cvGetWindowHandle("Mirror window");
			//RECT winSize;
			//GetClientRect(hwnd, &winSize);
			//Mat croppedImg = image(Rect(0, 0, winSize.right, winSize.bottom));

			// Mirror/flip the image
			Mat mirrorImg;
			flip(croppedImg, mirrorImg, flipY);


			// Show result in window
			imshow(windowName, croppedImg);
		}
		int key = waitKey(5);

		if (key == 27)//press esc to break loop
		{
			break;
		}

	}

	return 0;
}