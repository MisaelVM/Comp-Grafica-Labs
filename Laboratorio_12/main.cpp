#include <opencv2/opencv.hpp>
#include <iostream>

#include <cstdlib>
#include <ctime>

int main()
{
	// Creen un arreglo de 5x5 donde los numeros sean aleatorios del 1 al 10
	int arr[5][5]{};
	srand(static_cast<unsigned int>(time(nullptr)));
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			arr[i][j] = rand() % 10 + 1;

	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j)
			std::cout << arr[i][j] << "\t";
		std::cout << "\n";
	}

	cv::Mat img = cv::imread("test.jpg", cv::IMREAD_COLOR);
	// Muestren solo el canal azul de la imagen
	cv::Mat imgBlue = img.clone();
	for (int row = 0; row < imgBlue.rows; ++row) {
		for (int col = 0; col < imgBlue.cols; ++col) {
			cv::Vec3b& color = imgBlue.at<cv::Vec3b>(row, col);
			
			color[1] = 0;
			color[2] = 0;
		}
	}

	cv::imshow("Original Image", img);
	cv::imshow("Only Blue Channel", imgBlue);
	cv::waitKey();
	cv::destroyAllWindows();

	// Cambiar imagen a escala de grises
	cv::Mat imgGrayScale;
	cv::cvtColor(img, imgGrayScale, cv::COLOR_BGR2GRAY);

	cv::imshow("Original Image", img);
	cv::imshow("Grayscale Image", imgGrayScale);
	cv::waitKey();
	cv::destroyAllWindows();

	// Guardar una nueva imagen que contenga solo los canales rojo y verde
	cv::Mat imgRedGreen = img.clone();
	for (int row = 0; row < imgRedGreen.rows; ++row) {
		for (int col = 0; col < imgRedGreen.cols; ++col) {
			cv::Vec3b& color = imgRedGreen.at<cv::Vec3b>(row, col);

			color[0] = 0;
		}
	}

	cv::imshow("Original Image", img);
	cv::imshow("Only Red and Green Channels", imgRedGreen);
	cv::imwrite("testRG.jpg", imgRedGreen);
	cv::waitKey();
	cv::destroyAllWindows();

	// Guarden una nueva imagen que muestre la imagen en escala de grises
	cv::imshow("Original Image", img);
	cv::imshow("Grayscale Image", imgGrayScale);
	cv::imwrite("testGrayscale.jpg", imgGrayScale);
	cv::waitKey();
	cv::destroyAllWindows();
	
	return 0;
}
