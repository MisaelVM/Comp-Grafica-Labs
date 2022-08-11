#include <opencv2/opencv.hpp>
#include <iostream>

#include <vector>

cv::Mat imgDrawable;
cv::Point linePointA, linePointB;
void drawLineOnClick(int action, int x, int y, int flags, void* userdata);

int main()
{
	cv::Mat img = cv::imread("lab13_01.jpg", cv::IMREAD_UNCHANGED);

	// Cambien el tipo de color a RGB
	cv::Mat imgRGB;
	cv::cvtColor(img, imgRGB, cv::COLOR_BGR2RGB);

	cv::imshow("Original Image", img);
	cv::imshow("RGB Image", imgRGB);
	cv::waitKey();
	cv::destroyAllWindows();
	
	// Hagan girar una imagen
	cv::Point center((img.cols - 1) / 2.0, (img.rows - 1) / 2.0);
	cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, 45.0f, 1.0f);
	
	cv::Mat imgRotated;
	cv::warpAffine(imgRGB, imgRotated, rotationMatrix, imgRGB.size());

	cv::imshow("Original Image", img);
	cv::imshow("Rotated Image", imgRotated);
	cv::waitKey();
	cv::destroyAllWindows();

	// Dibujen un círculo sobre las caras de dicha figura y agreguen un texto que describa que es
	cv::Mat imgCircleText = imgRotated.clone();
	cv::circle(imgCircleText, cv::Point(305, 220), 50, cv::Scalar(0, 0, 255), 3, 8, 0);
	cv::putText(imgCircleText, "Perro", cv::Point(235, 145), cv::FONT_HERSHEY_COMPLEX, 1.5, cv::Scalar(0, 0, 255));
	cv::circle(imgCircleText, cv::Point(145, 220), 60, cv::Scalar(0, 255, 0), 3, 8, 0);
	cv::putText(imgCircleText, "Persona", cv::Point(40, 330), cv::FONT_HERSHEY_COMPLEX, 1.5, cv::Scalar(0, 255, 0));
	
	cv::imshow("Original Image", img);
	cv::imshow("Annotated Image", imgCircleText);
	cv::waitKey();
	cv::destroyAllWindows();

	// Guarden la figura con los círculos dibujados y el texto generado
	cv::imwrite("annotated_img.jpg", imgCircleText);

	// En una imagen distinta al anterior creen un pentágono sobre las caras de las
	// figuras y agreguen un texto que describa que es
	cv::Mat img2 = cv::imread("lab13_02.jpg", cv::IMREAD_UNCHANGED);

	cv::Mat img2Pentagon = img2.clone();
	std::vector<cv::Point> pointsA = { { 65, 310 }, { 255, 310 }, { 314, 129 }, { 160, 18 }, { 6, 129 } };
	cv::polylines(img2Pentagon, pointsA, true, cv::Scalar(255, 255, 0), 3, 8, 0);
	cv::putText(img2Pentagon, "Persona", cv::Point(55, 350), cv::FONT_HERSHEY_COMPLEX, 1.5, cv::Scalar(255, 255, 0));
	std::vector<cv::Point> pointsB = { { 425, 330 }, { 615, 330 }, { 674, 149 }, { 520, 38 }, { 366, 149 } };
	cv::polylines(img2Pentagon, pointsB, true, cv::Scalar(0, 255, 255), 3, 8, 0);
	cv::putText(img2Pentagon, "Perro", cv::Point(450, 370), cv::FONT_HERSHEY_COMPLEX, 1.5, cv::Scalar(0, 255, 255));

	cv::imshow("Original image", img2);
	cv::imshow("Pentagon image", img2Pentagon);
	cv::waitKey();
	cv::destroyAllWindows();
	cv::imwrite("annotated_pentagon_img.jpg", img2Pentagon);

	// Creen un programa que permita dibujar diferentes tipos de figuras geométricas con el uso
	// de eventos de mouse para poder hacer dibujos y luego guarden la imagen
	cv::Mat imgClear = img2.clone();
	imgDrawable = img2.clone();
	cv::namedWindow("Window");
	cv::setMouseCallback("Window", drawLineOnClick);

	int k = 0;
	while (k != 'q') {
		cv::imshow("Window", imgDrawable);
		k = cv::waitKey();
		switch (k) {
			case 'c': imgClear.copyTo(imgDrawable); break;
			case 's': cv::imwrite("annotated_custom_img.jpg", imgDrawable); break;
			default: break;
		}		
	}
	cv::destroyAllWindows();

	// Modifiquen el programa para agregar funcionalidades
	k = 0;
	bool enabled_channels[3] = { true, true, true };
	cv::Mat merged_img = img2.clone();
	while (k != 'q') {
		k = cv::waitKey();
		switch (k) {
			case 'r': enabled_channels[2] ^= true; break;
			case 'g': enabled_channels[1] ^= true; break;
			case 'b': enabled_channels[0] ^= true; break;
			case 's': cv::imwrite("extra_img.jpg", merged_img);
			default: break;
		}

		cv::Mat img_channels[3];
		cv::split(img2, img_channels);
		std::vector<cv::Mat> channels(3, cv::Mat::zeros(img2.rows, img2.cols, CV_8UC1));
		for (int i = 0; i < 3; ++i)
			if (enabled_channels[i])
				channels[i] = img_channels[i];
		cv::merge(channels, merged_img);
		cv::imshow("Window", merged_img);
	}
	cv::destroyAllWindows();

	return 0;
}

void drawLineOnClick(int action, int x, int y, int flags, void* userdata) {
	if (action == cv::EVENT_LBUTTONDOWN) {
		linePointA = cv::Point(x, y);
	}
	else if (action == cv::EVENT_LBUTTONUP) {
		linePointB = cv::Point(x, y);

		cv::line(imgDrawable, linePointA, linePointB, cv::Scalar(0, 255, 0), 3, 8, 0);
		cv::imshow("Window", imgDrawable);
	}
}
