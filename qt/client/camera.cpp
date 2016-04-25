#include "camera.h"
#include <boost/bind.hpp>
#include <QDebug>
Camera::Camera(QObject *parent, int dev_id) : QObject(parent),
    camera_on_(false),
    current_video_(NULL),
    dev_id_(dev_id)
{
    camera_timer_ = new QTimer() ;
    camera_timer_->setInterval(10);
    connect(camera_timer_,SIGNAL(timeout()),this , SLOT(timeOut())) ;
    qDebug()<<"init" ;
    camera_timer_->start();
}

Camera::~Camera()
{
    delete this->current_video_ ;
}

void Camera::timeOut()
{
    if(camera_on_==true)
    {
        emit getImageInfo(current_video_->getFrame());
    }
}

void Camera::ImageCallback(IplImage *rawImage)
{
    qDebug()<<"non" ;

    ///////////////////////////////////////
    // ÐèÒªÊ¹ÓÃµÄ±äÁ¿

    static IplImage*   yuvImage =NULL;
    static IplImage*   ImaskCodeBook = NULL;
    static codeBook*   cB;
    static unsigned    cbBounds[CHANNELS];
    uchar*      pColor; //YUV pointer
    static int         imageLen = 0;
    int         nChannels = CHANNELS;
    static int         minMod[CHANNELS];
    static int         maxMod[CHANNELS];


    //////////////////////////////////////////////////////////////////////////
    // ³õÊŒ»¯ž÷±äÁ¿
    static SeatManager nSM;
    static int i = 0;
    if(i == 0)
    {
        int mData[M_SIZE][4]={
            {100,200,0,50},
            {170,270,50,100},
            {190,290,105,155},
            {200,300,150,200},
            {150,240,200,250},
            {120,200,250,310},
            {100,170,310,350},
            {180,280,480,530}
        };

        nSM.Init(M_SIZE,mData);
        //cvNamedWindow("CodeBook");
        //cvNamedWindow("Raw");



        yuvImage = cvCreateImage(cvGetSize(rawImage), 8, 3);
        // žøyuvImage ·ÖÅäÒ»žöºÍrawImage ³ßŽçÏàÍ¬,8Î»3ÍšµÀÍŒÏñ
        ImaskCodeBook = cvCreateImage(cvGetSize(rawImage), IPL_DEPTH_8U, 1);
        // ÎªImaskCodeBook ·ÖÅäÒ»žöºÍrawImage ³ßŽçÏàÍ¬,8Î»µ¥ÍšµÀÍŒÏñ
        cvSet(ImaskCodeBook, cvScalar(255));
        // ÉèÖÃµ¥ÍšµÀÊý×éËùÓÐÔªËØÎª255,ŒŽ³õÊŒ»¯Îª°×É«ÍŒÏñ

        imageLen = rawImage->width * rawImage->height;
        cB = new codeBook[imageLen];
        // µÃµœÓëÍŒÏñÏñËØÊýÄ¿³€¶ÈÒ»ÑùµÄÒ»×éÂë±Ÿ,ÒÔ±ã¶ÔÃ¿žöÏñËØœøÐÐŽŠÀí

        for (int ii=0; ii<imageLen; ii++)
            // ³õÊŒ»¯Ã¿žöÂëÔªÊýÄ¿Îª0
            cB[ii].numEntries = 0;
        for (int ii=0; ii<nChannels; ii++)
        {
            cbBounds[ii] = 20;   // ÓÃÓÚÈ·¶šÂëÔªž÷ÍšµÀµÄ·§Öµ
            minMod[ii]   = 30;   // ÓÃÓÚ±³Ÿ°²î·Öº¯ÊýÖÐ
            maxMod[ii]   = 30;   // µ÷ÕûÆäÖµÒÔŽïµœ×îºÃµÄ·Öžî
        }
    }




    //////////////////////////////////////////////////////////////////////////
    // ¿ªÊŒŽŠÀíÊÓÆµÃ¿Ò»Ö¡ÍŒÏñ



        bool studying = false;
        //cvCvtColor(rawImage, yuvImage, CV_BGR2YCrCb);
        // É«²Ê¿ÕŒä×ª»»,œ«rawImage ×ª»»µœYUVÉ«²Ê¿ÕŒä,Êä³öµœyuvImage
        // ŒŽÊ¹²»×ª»»Ð§¹ûÒÀÈ»ºÜºÃ
        yuvImage = cvCloneImage(rawImage);

        if (i++ <= 30)
            // 30Ö¡ÄÚœøÐÐ±³Ÿ°Ñ§Ï°
        {
            studying = true;
            pColor = (uchar *)(yuvImage->imageData);
            // ÖžÏòyuvImage ÍŒÏñµÄÍšµÀÊýŸÝ
            for (int c=0; c<imageLen; c++)
            {
                M_Method::update_codebook(pColor, cB[c], cbBounds, nChannels,false);
                // ¶ÔÃ¿žöÏñËØ,µ÷ÓÃŽËº¯Êý,²¶×œ±³Ÿ°ÖÐÏà¹Ø±ä»¯ÍŒÏñ
                pColor += 3;
                // 3 ÍšµÀÍŒÏñ, ÖžÏòÏÂÒ»žöÏñËØÍšµÀÊýŸÝ
            }
            if (i== 30)
                // µœ30 Ö¡Ê±µ÷ÓÃÏÂÃæº¯Êý,ÉŸ³ýÂë±ŸÖÐ³ÂŸÉµÄÂëÔª
            {
                for (int c=0; c<imageLen; c++)
                    M_Method::clear_stale_entries(cB[c]);
                //outfile.close();
            }
        }
        else
        {
            uchar maskPixelCodeBook;
            uchar temp=0;
            pColor = (uchar *)((yuvImage)->imageData); //3 channel yuv image
            uchar *pMask = (uchar *)((ImaskCodeBook)->imageData); //1 channel image
            // ÖžÏòImaskCodeBook ÍšµÀÊýŸÝÐòÁÐµÄÊ×ÔªËØ
            for(int c=0; c<imageLen; c++)
            {
                maskPixelCodeBook = M_Method::background_diff(pColor, cB[c], nChannels, minMod, maxMod);
                *pMask++ = maskPixelCodeBook;
                M_Method::update_codebook(pColor, cB[c], cbBounds, nChannels,true);
                pColor += 3;
                // pColor ÖžÏòµÄÊÇ3ÍšµÀÍŒÏñ

            }
        }

        //cvShowImage("Raw", rawImage);

        cv::Mat newMask(ImaskCodeBook,true);
        cv::Mat newMat(rawImage,true);

        int num=50;
        int poly1_hull0=0;
        float perimScale = 20.0;
        CvRect bbs[50];
        CvPoint centers[50];
        //cvShowImage("codeBook1",ImaskCodeBook);

        M_Method::find_connected_components(ImaskCodeBook,poly1_hull0,perimScale,&num,bbs,centers);
        //cvShowImage("CodeBook", ImaskCodeBook);

        int pnum = 0;
        if(!studying)
        {

            for(int j = 0; j < nSM.Size(); j++)
            {
                if(i%2)
                {
                    nSM.Get(j).Judge(newMask);
                }
                    //cout<<i<<" ";
                    //nSM.Get(i).Judge(newMask);
                if(nSM.Get(j).GetJudge())
                    pnum++;
                //nSM.Get(j).Draw(newMat,5);
            }
        }
        this->current_people_num_ = pnum;
        emit updatePeopleNum(this->current_people_num_);
        //cv::imshow("new", newMat);

    //delete [] cB;
    //return 0;
}

void Camera::getCaptrueOn()
{
    current_video_ = new VideoDevice() ;
    this->current_video_->setImageCallBack(boost::bind(&Camera::ImageCallback,this,_1));
    current_video_->setDevNum(dev_id_);
    current_video_->openDevice();
    camera_on_ = true ;


}

void Camera::closeCaptrue()
{
    current_video_->closeDevice();
    delete current_video_ ;
    camera_on_ = false ;
}



