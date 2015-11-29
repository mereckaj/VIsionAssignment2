/*
 * KDH's code, might have made some changes. Can't remember.
 */
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

class Histogram
{
protected:
    cv::Mat mImage;
    int mNumberChannels;
    int* mChannelNumbers;
    int* mNumberBins;
    float mChannelRange[2];
public:
    Histogram( cv::Mat image, int number_of_bins )
    {
        mImage = image;
        mNumberChannels = mImage.channels();
        mChannelNumbers = new int[mNumberChannels];
        mNumberBins = new int[mNumberChannels];
        mChannelRange[0] = 0.0;
        mChannelRange[1] = 255.0;
        for (int count=0; count<mNumberChannels; count++)
        {
            mChannelNumbers[count] = count;
            mNumberBins[count] = number_of_bins;
        }
        //ComputeHistogram();
    }
    virtual void ComputeHistogram()=0;
    virtual void NormaliseHistogram()=0;
    static void Draw1DHistogram( cv::MatND histograms[], int number_of_histograms, cv::Mat& display_image )
    {
        int number_of_bins = histograms[0].size[0];
        double max_value=0, min_value=0;
        double channel_max_value=0, channel_min_value=0;
        for (int channel=0; (channel < number_of_histograms); channel++)
        {
            cv::minMaxLoc(histograms[channel], &channel_min_value, &channel_max_value, 0, 0);
            max_value = ((max_value > channel_max_value) && (channel > 0)) ? max_value : channel_max_value;
            min_value = ((min_value < channel_min_value) && (channel > 0)) ? min_value : channel_min_value;
        }
        float scaling_factor = ((float)256.0)/((float)number_of_bins);

        cv::Mat histogram_image((int)(((float)number_of_bins)*scaling_factor)+1,(int)(((float)number_of_bins)*scaling_factor)+1,CV_8UC3,cv::Scalar(255,255,255));
        display_image = histogram_image;
        cv::line(histogram_image,cv::Point(0,0),cv::Point(0,histogram_image.rows-1),cv::Scalar(0,0,0));
        line(histogram_image,cv::Point(histogram_image.cols-1,histogram_image.rows-1),cv::Point(0,histogram_image.rows-1),cv::Scalar(0,0,0));
        int highest_point = static_cast<int>(0.9*((float)number_of_bins)*scaling_factor);
        for (int channel=0; (channel < number_of_histograms); channel++)
        {
            int last_height = 0;
            for( int h = 0; h < number_of_bins; h++ )
            {
                float value = histograms[channel].at<float>(h);
                int height = static_cast<int>(value*highest_point/max_value);
                if (h > 0)
                    line(histogram_image,cv::Point((int)(((float)(h-1))*scaling_factor)+1,(int)(((float)number_of_bins)*scaling_factor)-last_height),
                         cv::Point((int)(((float)h)*scaling_factor)+1,(int)(((float)number_of_bins)*scaling_factor)-height),
                         cv::Scalar(channel==0?255:0,channel==1?255:0,channel==2?255:0));
                last_height = height;
            }
        }
    }
};
class OneDHistogram : public Histogram
{
private:
    cv::MatND mHistogram[3];
public:
    OneDHistogram( cv::Mat image, int number_of_bins ) :
            Histogram( image, number_of_bins )
    {
        ComputeHistogram( );
    }
    void ComputeHistogram( )
    {
        std::vector<cv::Mat> image_planes(mNumberChannels);
        split(mImage, image_planes);
        for (int channel=0; (channel < mNumberChannels); channel++)
        {
            const float* channel_ranges = mChannelRange;
            calcHist(&(image_planes[channel]), 1, mChannelNumbers, cv::Mat(), mHistogram[channel], 1 , mNumberBins, &channel_ranges);
        }
    }
    void SmoothHistogram( )
    {
        for (int channel=0; (channel < mNumberChannels); channel++)
        {
            cv::MatND temp_histogram = mHistogram[channel].clone();
            for(int i = 1; i < mHistogram[channel].rows - 1; ++i)
            {
                mHistogram[channel].at<float>(i) = (temp_histogram.at<float>(i-1) + temp_histogram.at<float>(i) + temp_histogram.at<float>(i+1)) / 3;
            }
        }
    }
    cv::MatND getHistogram(int index)
    {
        return mHistogram[index];
    }
    void NormaliseHistogram()
    {
        for (int channel=0; (channel < mNumberChannels); channel++)
        {
            normalize(mHistogram[channel],mHistogram[channel],1.0);
        }
    }
    cv::Mat BackProject(cv::Mat& image )
    {
        cv::Mat result = image.clone();
        if (mNumberChannels == 1)
        {
            const float* channel_ranges[] = { mChannelRange, mChannelRange, mChannelRange };
            for (int channel=0; (channel < mNumberChannels); channel++)
            {
                calcBackProject(&image,1,mChannelNumbers,*mHistogram,result,channel_ranges,255.0);
            }
        }
        else
        {
        }
        return result;
    }
    void Draw( cv::Mat& display_image )
    {
        Draw1DHistogram( mHistogram, mNumberChannels, display_image );
    }
};
class ColourHistogram : public Histogram
{
private:
    cv::MatND mHistogram;
public:
    ColourHistogram( cv::Mat image, int number_of_bins ) :
            Histogram( image, number_of_bins )
    {
        ComputeHistogram();
    }
    void ComputeHistogram()
    {
        const float* channel_ranges[] = { mChannelRange, mChannelRange, mChannelRange };
        calcHist(&mImage, 1, mChannelNumbers, cv::Mat(), mHistogram, mNumberChannels, mNumberBins, channel_ranges);
    }
    void NormaliseHistogram()
    {
        cv::normalize(mHistogram,mHistogram,1.0);
    }
    cv::Mat BackProject( cv::Mat& image )
    {
        cv::Mat result = image.clone();
        const float* channel_ranges[] = { mChannelRange, mChannelRange, mChannelRange };
        cv::calcBackProject(&image,1,mChannelNumbers,mHistogram,result,channel_ranges,255.0);
        return result;
    }
    cv::MatND getHistogram()
    {
        return mHistogram;
    }
};
class HueHistogram : public Histogram
{
private:
    cv::MatND mHistogram;
    int mMinimumSaturation, mMinimumValue, mMaximumValue;
#define DEFAULT_MIN_SATURATION 25
#define DEFAULT_MIN_VALUE 25
#define DEFAULT_MAX_VALUE 230
public:
    HueHistogram( cv::Mat image, int number_of_bins, int min_saturation=DEFAULT_MIN_SATURATION, int min_value=DEFAULT_MIN_VALUE, int max_value=DEFAULT_MAX_VALUE ) :
            Histogram( image, number_of_bins )
    {
        mMinimumSaturation = min_saturation;
        mMinimumValue = min_value;
        mMaximumValue = max_value;
        mChannelRange[1] = 180.0;
        ComputeHistogram();
    }
    void ComputeHistogram()
    {
        cv::Mat hsv_image, hue_image, mask_image;
        cvtColor(mImage, hsv_image, CV_BGR2HSV);
        inRange( hsv_image, cv::Scalar( 0, 0, mMinimumValue ), cv::Scalar( 180, 256, mMaximumValue ), mask_image );
        int channels[]={0,0};
        hue_image.create( mImage.size(), mImage.depth());
        mixChannels( &hsv_image, 1, &hue_image, 1, channels, 1 );
        const float* channel_ranges = mChannelRange;
        calcHist( &hue_image,1,0,mask_image,mHistogram,1,mNumberBins,&channel_ranges);
    }
    void NormaliseHistogram()
    {
        normalize(mHistogram,mHistogram,0,255,CV_MINMAX);
    }
    cv::Mat BackProject( cv::Mat& image )
    {
        cv::Mat result = image.clone();
        const float* channel_ranges = mChannelRange;
        calcBackProject(&image,1,mChannelNumbers,mHistogram,result,&channel_ranges,255.0);
        return result;
    }
    cv::MatND getHistogram()
    {
        return mHistogram;
    }
    void Draw( cv::Mat& display_image )
    {
        Draw1DHistogram( &mHistogram, 1, display_image );
    }
};