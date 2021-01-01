#include <iostream>
#include <sstream>
#include <map>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;

static void show_usage() {
    cout << "Available commands:" << endl
         << " help, h -- this help" << endl
         << " load, ld <name> <filename> -- load image file from <filename> in JPG format" << endl
         << " store, s <name> <filename> -- save image to file <filename> in JPG format" << endl
         << " blur <from_name> <to_name> <size> -- smoothing image" << endl
         << " resize <from_name> <to_name> <new_width> <new_height> -- resize image" << endl
         << " exit, quit, q -- leave program" << endl;
}

int main(int argc, char** argv ) {
    cout << "Test of opencv" << endl;
    string helpStr;
    cout << "Input command (h, help -- this help)" << endl;
    show_usage();
    string commandStr;
    string image_name = string();
    map<string, Mat> images;

    for( commandStr = string(); commandStr.compare("q") && commandStr.compare("exit") && commandStr.compare("quit"); getline(cin, commandStr) ) {
        cout << "Command is " << commandStr << endl;
        stringstream commandStream( commandStr );
        if( !commandStr.compare("h") || !commandStr.compare("help") )
            show_usage();
        else if( !commandStr.substr(0, 2).compare("ld") || !commandStr.substr(0, 4).compare("load") ) {
            string command, name, file_name;
            commandStream >> command >> name >> file_name;
            cout << "Loading image " << name << " from " << file_name << endl;
            Mat image;
            image = imread( file_name.c_str() );
            if ( !image.data ) {
                cout << "No image data " << endl;
                continue;
            }
            map<string, Mat>::iterator imp = images.find( name );
            if( imp != images.end() ) {
                images.erase(imp);
            }
            images.insert( pair<string, Mat>(name, image) );
        }
        else if( !commandStr.substr(0, 1).compare("s") || !commandStr.substr(0, 5).compare("store") ) {
            string command, name, file_name;
            commandStream >> command >> name >> file_name;
            cout << "Save image " << name << " to " << file_name << endl;
            map<string, Mat>::iterator imp = images.find( name );
            if( imp == images.end() ) {
                continue;
            }
            vector<int> compression_params;
            compression_params.push_back( IMWRITE_JPEG_QUALITY );
            compression_params.push_back( 87 );
            imwrite( file_name.c_str(), images[name], compression_params );
        }
        else if( !commandStr.substr(0, 4).compare("blur") ) {
            string command, from_name, to_name;
            int imsize;
            commandStream >> command >> from_name >> to_name >> imsize;
            map<string, Mat>::iterator imp = images.find( from_name );
            if( imp == images.end() ) {
                continue;
            }
            map<string, Mat>::iterator impdst = images.find( to_name );
            if( impdst != images.end() )
                images.erase( impdst );
            Mat simage = images[from_name];
            Mat dstimage;
            Size imageS( imsize, imsize );
            blur( simage, dstimage, imageS );
            images.insert( pair<string, Mat>( to_name, dstimage ) );
        }
        else if( !commandStr.substr(0, 6).compare("resize") ) {
            string command, from_name, to_name;
            int imwidth, imheight;
            commandStream >> command >> from_name >> to_name >> imwidth >> imheight;
            map<string, Mat>::iterator imp = images.find( from_name );
            if( imp == images.end() ) {
                continue;
            }
            map<string, Mat>::iterator impdst = images.find( to_name );
            if( impdst != images.end() )
                images.erase( impdst );
            Mat simage = images[from_name];
            Mat dstimage;
            Size imageS( imwidth, imheight );
            resize( simage, dstimage, imageS );
            images.insert( pair<string, Mat>( to_name, dstimage ) );
        }
    }
    return 0;
}
