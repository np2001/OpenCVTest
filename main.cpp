//-------------------------------------------------------------------------------
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <map>
//-------------------------------------------------------------------------------
using namespace cv;
using namespace std;
//-------------------------------------------------------------------------------

std::map<string, Mat> Images;

string GetParam(const vector<string>& params, uint index)
{
    if (index < params.size())
    {
        return params[index];
    }

    return string();
}

void InvalidParams()
{
    cout << "Invalid params" << endl;
}

void NoImageFound()
{
    cout << "No image found" << endl;
}

bool GetImageByName(const string& image_name, cv::Mat& image)
{
    bool res = false;
    auto image_iter = Images.find(image_name);
    if (image_iter != Images.end())
    {
        image = image_iter->second;
        res = true;
    }

    return res;
}

bool ProcessCommand(string s)
{
    bool exit = false;

    vector<string> params;
    std::string delimiter = " ";

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        params.push_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }

    if (s.length())
        params.push_back(s);

    string command = GetParam(params, 0);
    if (command == "load" || command == "ld")
    {
        string image_name = GetParam(params, 1);
        string image_filename = GetParam(params, 2);
        if (image_name.length() && image_filename.length())
        {
            Mat image = cv::imread(image_filename);
            if (!image.empty())
            {
                Images[image_name] = image;
            }
            else
            {
                cout << "Error loading image" << endl;
            }
        }
        else
        {
            InvalidParams();
        }
    }
    else {
        if (command == "store" || command == "s")
        {
            string image_name = GetParam(params, 1);
            string image_filename = GetParam(params, 2);

            if (image_name.length() && image_filename.length())
            {
                Mat image;
                if (GetImageByName(image_name, image))
                {
                    bool result = false;
                    try
                    {
                        result = imwrite(image_filename, image);
                    }
                    catch (const cv::Exception& ex)
                    {

                    }

                    if (!result)
                        cout << "Error writing image" << std::endl;
                }
                else
                {
                    NoImageFound();
                }
            }
            else
            {
                InvalidParams();
            }
        }
        else

            if (command == "blur")
            {
                string in_image_name = GetParam(params, 1);
                string out_image_name = GetParam(params, 2);
                int blur_size = std::stoi(GetParam(params, 3));

                if (in_image_name.length() && out_image_name.length() && (blur_size % 2) == 1)
                {
                    Mat image;
                    if (GetImageByName(in_image_name, image))
                    {
                        Mat out_image;
                        blur(image, out_image, Size(blur_size, blur_size));
                        Images[out_image_name] = out_image;
                    }
                    else
                    {
                        NoImageFound();
                    }
                }
                else
                {
                    InvalidParams();
                }
            }
            else
                if (command == "resize")
                {
                    string in_image_name = GetParam(params, 1);
                    string out_image_name = GetParam(params, 2);
                    int new_width = std::stoi(GetParam(params, 3));
                    int new_height = std::stoi(GetParam(params, 4));

                    if (in_image_name.length() && out_image_name.length() && new_width > 0 && new_height > 0)
                    {
                        Mat image;
                        if (GetImageByName(in_image_name, image))
                        {
                            Mat out_image;
                            resize(image, out_image, Size(new_width, new_height));
                            Images[out_image_name] = image;
                        }
                        else
                        {
                            NoImageFound();
                        }
                    }
                    else
                    {
                        InvalidParams();
                    }
                }
                else {
                    if (command == "help" || command == "h")
                    {
                        cout << "refer to https://docs.google.com/document/d/1dQLhPw0k8gUB8ej3yRTwYEi5X2DjCn5Y43Hrkd7Km9Y/edit#heading=h.s4s6ny9ff6g0 for params description" << endl;
                    }
                    else {
                        if (command == "exit" || command == "quit" || command == "q")
                        {
                            exit = true;
                        }
                    }
                }
    }

    return exit;
}

int main(int argc, char *argv[])
{
    std::string s;

    do
    {
        cout << ">>>";
        std::getline(std::cin, s);
    }
    while (!ProcessCommand(s));

    return 0;
}
//-------------------------------------------------------------------------------
