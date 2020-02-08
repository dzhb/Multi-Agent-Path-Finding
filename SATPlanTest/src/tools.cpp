//
// Created by dzhb on 2020/2/7.
//
#include<fstream>
#include <ctime>
#include "../inc/tools.h"


bool graphCheck(std::vector<std::vector<int>> pathImg, std::vector<Agent> &Agents) {
    for (int agt = 0; agt < Agents.size(); agt++) {
        if (pathImg[Agents[agt].startVertex[0]][Agents[agt].startVertex[1]] != VERTEX) {
            printf("start point of an agent cannot be an obstacle!!!");
            exit(0);
        }
        if (pathImg[Agents[agt].endVertex[0]][Agents[agt].endVertex[1]] != VERTEX) {
            printf("end point of an agent cannot be an obstacle!!!");
            exit(0);
        }
    }

    for (int agt = 0; agt < Agents.size(); agt++) {
        for (int agt2 = agt + 1; agt2 < Agents.size(); agt2++) {
            if (Agents[agt].startVertex == Agents[agt2].startVertex) {
                printf("Agent %s coliide with Agent %s in start point", Agents[agt].name.c_str(),
                       Agents[agt2].name.c_str());
                exit(0);
            }
            if (Agents[agt].endVertex == Agents[agt2].endVertex) {
                printf("Agent %s coliide with Agent %s in end point", Agents[agt].name.c_str(),
                       Agents[agt2].name.c_str());
                exit(0);
            }
        }
    }

}


/**
 * Shortest distance between start point and end point
 * @param Agents
 * @param startVertexs
 * @param endVertexs
 * @return
 */
int minDistance(const std::vector<Agent> &Agents) {
    int min = sqrt(pow(Agents[0].endVertex[0] - Agents[0].startVertex[0], 2)
                   + pow(Agents[0].endVertex[1] - Agents[0].startVertex[1], 2)) - 1;;
    for (int agt = 1; agt < Agents.size(); agt++) {
        int distance = sqrt(pow(Agents[agt].endVertex[0] - Agents[agt].startVertex[0], 2)
                            + pow(Agents[agt].endVertex[1] - Agents[agt].startVertex[1], 2)) - 1;
        if (distance < min)
            min = distance;
        return min;
    }
}

int maxDistance(const std::vector<Agent> &Agents) {
    int max = sqrt(pow(Agents[0].endVertex[0] - Agents[0].startVertex[0], 2)
                   + pow(Agents[0].endVertex[1] - Agents[0].startVertex[1], 2)) - 1;;
    for (int agt = 1; agt < Agents.size(); agt++) {
        int distance = sqrt(pow(Agents[agt].endVertex[0] - Agents[agt].startVertex[0], 2)
                            + pow(Agents[agt].endVertex[1] - Agents[agt].startVertex[1], 2)) - 1;
        if (distance > max)
            max = distance;
        return max;
    }
}


//Get the maximum moving range of agent in timsstep time
std::vector<std::vector<int>> getRange(int timeStep, int start_X, int start_Y, int rows, int cols) {
    timeStep++;
    std::vector<int> range_x = {start_X - timeStep, start_X + timeStep};
    std::vector<int> range_y = {start_Y - timeStep, start_Y + timeStep};
    if (range_x[0] < 0) range_x[0] = 0;
    if (range_y[0] < 0) range_y[0] = 0;
    if (range_x[1] > rows) range_x[1] = rows;
    if (range_y[1] > cols) range_y[1] = cols;
    return {range_x, range_y};
}


/**
 * 将数组存储到文本文件
 * @param imageToVector 数组
 * @param filename 文件地址和文件名
 */
void vectorsToFile(std::vector<std::vector<int>> imageToVector, std::string filename) {
    std::ofstream savefile(filename, std::ios::out);//新建一个文件
    for (int i = 0; i < imageToVector.size(); i++) {
        for (int j = 0; j < imageToVector[i].size(); j++) {
            savefile << imageToVector[i][j];
        }
        savefile << "\n";
    }
    savefile.close();
}

//图片转数组
std::vector<std::vector<int>> imageToVector(cv::Mat image) {
    int rows = image.rows;
    int cols = image.cols;
    std::vector<int> vec1;
    std::vector<std::vector<int>> vec2;
    for (int i = 0; i < rows; i++)//双层循环，遍历图像的像素点
    {
        for (int j = 0; j < cols; j++) {
            int bin_val = image.at<uchar>(i, j);
            if (bin_val <= 240) {
                vec1.push_back(COLOR_BLACK);
            } else {
                vec1.push_back(COLOR_WHITE);
            }
        }
        vec2.push_back(vec1);
        vec1.clear();
    }
    return vec2;

}

//将数组转为图片
cv::Mat vectorToImage(std::vector<std::vector<int>> vectorImage) {
    cv::Mat rgbimg;
    int rows = vectorImage.size();
    int cols = vectorImage.at(0).size();
    std::cout << "rows2:" << rows << std::endl;
    std::cout << "cols2:" << cols << std::endl;
//    if (rows > 0 && cols > 0) {
//        cv::Mat M(rows, cols, CV_8UC1);
//        for (int i = 0; i < rows; i++) {
//            uchar *p = M.ptr<uchar>(i);
//            for (int j = 0; j < cols; j++) {
//                if (vectorImage.at(i).at(j) == COLOR_BLACK) {
//                    p[j] = 0;
//                } else if (vectorImage.at(i).at(j) == COLOR_WHITE) {
//                    p[j] = 255;
//                } else {
//                    p[j] = 100;
////                    rgbimg.at<cv::Vec3b>(i,j)[0] = 128;
////                    rgbimg.at<cv::Vec3b>(i,j)[1] = 0;
////                    rgbimg.at<cv::Vec3b>(i,j)[2] = 0;
//                }
//            }
//        }
//
//        cvtColor(M, rgbimg, cv::COLOR_GRAY2RGB);
//
//
//        for (int i = 0; i < rows; i++) {
////            uchar *p = M.ptr<uchar>(i);
//            for (int j = 0; j < cols; j++) {
//                if (vectorImage.at(i).at(j) == COLOR_RED) {
//                    rgbimg.at<cv::Vec3b>(i, j)[0] = 0;//blue 通道
//                    rgbimg.at<cv::Vec3b>(i, j)[1] = 0;//green 通道
//                    rgbimg.at<cv::Vec3b>(i, j)[2] = 255;//red 通道
//                }
//            }
//        }


//        rgbimg = M;

    cv::Mat img(rows, cols, CV_8UC1);
    uchar *ptmp = NULL;
    for (int i = 0; i < rows; ++i) {
        ptmp = img.ptr<uchar>(i);

        for (int j = 0; j < cols; ++j) {
//            ptmp[j] = vectorImage[i][j];
            if (vectorImage.at(i).at(j) == COLOR_BLACK) {
                ptmp[j] = 0;
            } else if (vectorImage.at(i).at(j) == COLOR_WHITE) {
                ptmp[j] = 255;
            } else {
                ptmp[j] = 100;
            }
        }
    }

    return img;


}


//    return rgbimg;
//}

cv::Mat ImageResize(const cv::Mat &img, int height, int width) {
    double rows = img.rows, cols = img.cols;
    float datai = (float) rows / height, dataj = (float) cols / width;
    cv::Mat outPut = cv::Mat::zeros(height, width, CV_8UC1);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            outPut.at<uchar>(i, j) = img.at<uchar>(round(i * datai), round(j * dataj));
        }
    }
    return outPut;
}

cv::Mat convertTo3Channels(const cv::Mat &binImg) {
    cv::Mat three_channel = cv::Mat::zeros(binImg.rows, binImg.cols, CV_8UC3);
    std::vector<cv::Mat> channels;
    for (int i = 0; i < 3; i++) {
        channels.push_back(binImg);
    }
    merge(channels, three_channel);
    return three_channel;
}

cv::Mat DrawPath(cv::Mat srcImg, std::vector<Agent> &Agents,int zoom) {

    int agentNum = Agents.size();
    int times = Agents[0].path.size();

    int ch = srcImg.channels();
    cv::Mat src;
    if (ch == 1) src = convertTo3Channels(srcImg);
    else src = srcImg;
    int height = src.rows;
    int width = src.cols;

    std::vector<std::vector<int>> colorNum;
    srand(std::time(0));
    for (int agt = 0; agt < agentNum; agt++){
        std::vector<int> c = {rand() % 255, rand() % 255, rand() % 255};
        colorNum.push_back(c);
    }

    for (int t = 0; t < times; t++) {
        cv::Mat tempSrc = src.clone();
        for (int agt = 0; agt < agentNum; agt++) {
            cv::Vec3b bgr = tempSrc.at<cv::Vec3b>(Agents[agt].path[t][0],Agents[agt].path[t][1]);
            bgr[0] = colorNum[agt][0];
            bgr[1] = colorNum[agt][1];
            bgr[2] = colorNum[agt][2];
            tempSrc.at<cv::Vec3b>(Agents[agt].path[t][0],Agents[agt].path[t][1]) = bgr;
         }
        cv::resize(tempSrc,tempSrc,cv::Size(tempSrc.cols * zoom, tempSrc.rows *zoom),0,0,CV_INTER_AREA);

        cv::imwrite("/media/dzhb/DataDisk/ClionProjects/SATPlanTest/pathImage/t"+std::to_string(t)+".jpg",tempSrc);

//        for (int row = 0; row < height; row++) {
//            for (int col = 0; col < width; col++) {
//                cv::Vec3b bgr = src.at<cv::Vec3b>(row, col);
//                bgr[0] = 255 - k;
//                bgr[1] = 255 - k;
//                bgr[2] = 255 - k;
//                src.at<cv::Vec3b>(row, col) = bgr;
//            }
//        }
    }

    return src;
}
