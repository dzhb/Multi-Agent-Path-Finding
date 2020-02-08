
#include <opencv2/opencv.hpp>
#include "Agent.h"


#define COLOR_WHITE 0
#define COLOR_BLACK 1
#define COLOR_RED 2

#define VERTEX 0
#define OBSTACLE 1

bool graphCheck(std::vector<std::vector<int>> pathImg, std::vector<Agent> &Agents);

int minDistance(const std::vector<Agent>& Agents);
int maxDistance(const std::vector<Agent>& Agents);
std::vector<std::vector<int>> getRange(int timeStep, int start_X, int start_Y, int rows, int cols);

void vectorsToFile(std::vector<std::vector<int>> imageToVector, std::string filename);

//图片转数组
std::vector<std::vector<int>> imageToVector(cv::Mat image);

//将数组转为图片
cv::Mat vectorToImage(std::vector<std::vector<int>> vectorImage);


cv::Mat ImageResize(const cv::Mat &img, int height, int width);

//将单通道图像转为三通道图像
cv::Mat convertTo3Channels(const cv::Mat& binImg);

cv::Mat DrawPath(cv::Mat srcImage, std::vector<Agent>& Agents,int zoom);