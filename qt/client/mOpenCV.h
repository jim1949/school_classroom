#ifndef MOPENCV_H
#define MOPENCV_H

#include<vector>
#define M_COLOR_R 2
#define M_COLOR_G 1
#define M_COLOR_B 0
#define M_JUDGE_NOTHING 0
#define M_JUDGE_SOMETHING 255

#define M_SIZE 8

#define CHANNELS 3
#define CVCLOSE_ITR 1
#define CVCONTOUR_APPROX_LEVEL 2

enum M_DRAWTYPE
{
	M_DRAWTYPE_CENTER,
	M_DRAWTYPE_UPCENTER,
	M_DRAWTYPE_DOWNCENTER,
	M_DRAWTYPE_LEFTCENTER,
	M_DRAWTYPE_RIGHTCENTER,
	M_DRAWTYPE_LEFTTOP,
	M_DRAWTYPE_RIGHTTOP,
	M_DRAWTYPE_LEFTBOTTOM,
	M_DRAWTYPE_RIGHTBOTTOM
};

typedef struct ce{
	uchar learnHigh[CHANNELS];
	//
	uchar learnLow[CHANNELS];
	uchar max[CHANNELS];
	uchar min[CHANNELS];
	int t_last_update;
	int stale;
}code_element;

typedef struct code_book{
	code_element **cb;
	int numEntries;
	int t;
}codeBook;


class Vector2f
{
private :
	int mx,my;
public:
	Vector2f(int xx,int yy){mx=xx;my=yy;}
	Vector2f(){mx=0;my=0;}
	~Vector2f(){}
	int x(){return mx;}
	int y(){return my;}
	void Set(int xx,int yy){mx=xx;my=yy;}
};

class Drawer
{
public:
	static void DrawRect(cv::Mat &mat,Vector2f center,int height,int width,int quality,M_DRAWTYPE drawType)
	{
		switch(drawType)
		{
		case M_DRAWTYPE_CENTER:
			{
				center.Set(center.x()-height/2,center.y()-width/2);
				break;
			}
		case M_DRAWTYPE_UPCENTER:
			{
				center.Set(center.x(),center.y()-width/2);
				break;
			}
		case M_DRAWTYPE_DOWNCENTER:
			{
				center.Set(center.x()-height,center.y()-width/2);
				break;
			}
		case M_DRAWTYPE_LEFTCENTER:
			{
				center.Set(center.x()-height/2,center.y());
				break;
			}
		case M_DRAWTYPE_RIGHTCENTER:
			{
				center.Set(center.x()-height/2,center.y()-width);
				break;
			}
		case M_DRAWTYPE_LEFTTOP:
			{
				center.Set(center.x(),center.y());
				break;
			}
		case M_DRAWTYPE_RIGHTTOP:
			{
				center.Set(center.x(),center.y()-width);
				break;
			}
		case M_DRAWTYPE_LEFTBOTTOM:
			{
				center.Set(center.x()-height,center.y());
				break;
			}
		case M_DRAWTYPE_RIGHTBOTTOM:
			{
				center.Set(center.x()-height,center.y()-width);
				break;
			}

		}
		for(int i=center.x();i<center.x()+height;i++)
		{
			for(int j=center.y();j<center.y()+width;j++)
			{
				if(i<0||i>=mat.rows||j<0||j>=mat.cols)
				{
					;
				}
				else
				{
					if(i<center.x()+quality||i>center.x()+height-quality||j<center.y()+quality||j>center.y()+width-quality)
					{
						mat.at<cv::Vec3b>(i,j)[M_COLOR_B]=0;
						mat.at<cv::Vec3b>(i,j)[M_COLOR_G]=0;
						mat.at<cv::Vec3b>(i,j)[M_COLOR_R]=255;
					}
				}
			}

		}
	
	}
};

class Seat{
private:
	static const bool filter = true;
	static const int fNum = 35;
	
	double fRange;
	double decay;

	double abusolut;
	double judgeRange;

	int left,right,up,down;
	
	bool judge;
	Seat *obstruct;
	std::vector<bool> history;
public:
	Seat(){
		fRange = 0.8;decay = 0.98; abusolut = 1;judgeRange = 0.08;
		judge = false;obstruct = NULL;
	}
	~Seat() {}
	void SetArea(int u,int d,int l,int r) {left=l;right=r;up=u;down=d;}
	void SetObstruct(Seat *ob) {obstruct = ob;}
	bool GetJudge() {return judge;}
	std::vector<int> getRange() {std::vector<int> m;m.push_back(up);m.push_back(down);m.push_back(left);m.push_back(right);return m;}
	void Draw(cv::Mat &mat,int quality)
	{
		Drawer::DrawRect(mat,Vector2f(up,left),down-up,right-left,quality,M_DRAWTYPE_LEFTTOP);
	}
	bool Judge(cv::Mat img)
	{
		bool mJudge;
		int bottom = down;
		int n = 0;
		if(obstruct!=NULL)
		{
			if(obstruct->GetJudge())
			{
				bottom *=abusolut;
			}
		}
		for(int i=up;i<bottom;i++)
		{
			for(int j=left;j<right;j++)
			{

				if(img.at<uchar>(i,j)==M_JUDGE_SOMETHING)
					n++;
			}
		}
		std::cout<<n<<" "<<(right-left)*(bottom-up)<<" "<<n*1.0/(right-left)/(bottom-up)<<std::endl;
		if(n*1.0/(right-left)/(bottom-up)>judgeRange)
		{
			mJudge = true;
		}
		else
		{
			mJudge = false;
		}

		if(filter)
		{
			if(history.empty())
			{
				for(int i = 0; i < fNum; i++)
				{
					history.push_back(false);
				}
			}
			history.erase(history.begin());
			history.push_back(mJudge);
			double count_n = 0;
			double max_n = 0;
			for(int i = 0; i < history.size(); i++)
			{
				count_n *= decay;
				max_n *= decay;
				max_n += 1;

				if(history[i])
				{
					count_n += 1;
				}
				//std::cout<<history[i]<<" ";
			}
			//std::cout<<std::endl;
			if(count_n/max_n > fRange)
			{
				judge = true;
			}
			else
			{
				judge = false;
			}
		}
		else
		{
			judge = mJudge;
		}
		if(obstruct != NULL)
		{
			if(judge && obstruct->GetJudge())
			{
				judge = false;
			}
		}
		return judge;
	}
	friend class SeatManager;
};



class SeatManager
{
private:
	std::vector<Seat> SeatSeq;
	int size;
	
public:
	~SeatManager() {}
    void Init(int s,int mData[M_SIZE][4])
	{
		
		size = s;
		for(int i = 0; i < s; i++)
		{
			Seat nSeat;
            nSeat.SetArea(mData[i][0],mData[i][1],mData[i][2],mData[i][3]);
			SeatSeq.push_back(nSeat);
		}
		SeatSeq[2].SetObstruct(&SeatSeq[3]);
	}
	Seat &Get(int i)
	{
		return SeatSeq[i];
	}
	int Size()
	{
		return size;
	}
};


class M_Method
{
public:
	static int update_codebook(
		uchar *p,
		codeBook &c,
		unsigned *cbBounds,
		int numChannels,
		bool simple
		){
			if(c.numEntries==0) 
			{
				c.t=0;
			}
			c.t+=1;
			int diff[3]={0};
			unsigned int hight[3],low[3];
			int n;
			for(n=0;n<numChannels;n++)
			{
				hight[n]=*(p+n)+*(cbBounds+n);
				if(hight[n]>255) hight[n]=255;
				low[n]=*(p+n)-*(cbBounds+n);
				if(low[n]<0) low[n]=0;
				//std::cout<<low[n]<<std::endl;
			}
			int matchChannel;
			int i;
			//
			for(i=0;i<c.numEntries;i++)
			{
				matchChannel=0;
				for(n=0;n<numChannels;n++)
				{
					if((c.cb[i]->learnLow[n]<=*(p+n))&&
						(*(p+n)<=c.cb[i]->learnHigh[n]))
					{
						matchChannel++;
						//std::cout<<abs(int(c.cb[i]->learnLow[n]-*(p+n)))<<std::endl;
					}
					else if(simple)
					{
						//std::cout<<abs(int(c.cb[i]->min[n]-*(p+n)))<<std::endl;
						int tempnum=MIN(abs(int(c.cb[i]->min[n]-*(p+n))),abs(int(c.cb[i]->max[n]-*(p+n))));
						//std::cout<<tempnum<<std::endl;
						if(diff[n]==0) diff[n]=tempnum;
						else
						{
							if(diff[n]>tempnum) diff[n]=tempnum;
							//std::cout<<diff[n]<<std::endl;
						}
					}
				}
				if(matchChannel==numChannels)
				{
					c.cb[i]->t_last_update=c.t;
					//
					for(n=0;n<numChannels;n++)
					{
						if(c.cb[i]->max[n]<*(p+n))
						{
							c.cb[i]->max[n]=*(p+n);
						}
						else if(c.cb[i]->min[n]>*(p+n))
						{
							c.cb[i]->min[n]=*(p+n);
						}
					}
					if(simple)
					{
						for(n=0;n<numChannels;n++)
						{
							diff[n]=0;
						}
					}
					break;
				}
			}
			if(i==c.numEntries)
			{
				if(!simple)
				{
					code_element **foo=new code_element*[c.numEntries+1];
					for(int ii=0;ii<c.numEntries;ii++)
					{
						foo[ii]=c.cb[ii];
					}
					foo[c.numEntries]=new code_element;
					if(c.numEntries) delete [] c.cb;
					c.cb=foo;
					for(n=0;n<numChannels;n++)
					{
						c.cb[c.numEntries]->learnHigh[n]=hight[n];
						c.cb[c.numEntries]->learnLow[n]=low[n];
						c.cb[c.numEntries]->max[n]=*(p+n);
						c.cb[c.numEntries]->min[n]=*(p+n);
					}
					c.cb[c.numEntries]->t_last_update=c.t;
					c.cb[c.numEntries]->stale=0;
					c.numEntries+=1;
				}
				else
				{
					matchChannel=0;
					for(n=0;n<numChannels;n++)
					{
						if(diff[n]>0&&diff[n]<30) matchChannel++;
					}
					if(matchChannel==numChannels)
					{
						code_element **foo=new code_element*[c.numEntries+1];
						for(int ii=0;ii<c.numEntries;ii++)
						{
							foo[ii]=c.cb[ii];
						}
						foo[c.numEntries]=new code_element;
						if(c.numEntries) delete [] c.cb;
						c.cb=foo;
						for(n=0;n<numChannels;n++)
						{
							c.cb[c.numEntries]->learnHigh[n]=hight[n];
							c.cb[c.numEntries]->learnLow[n]=low[n];
							c.cb[c.numEntries]->max[n]=*(p+n);
							c.cb[c.numEntries]->min[n]=*(p+n);
						}
						c.cb[c.numEntries]->t_last_update=c.t;
						c.cb[c.numEntries]->stale=0;
						c.numEntries+=1;
					}
				}
			}
			//overhead
			for(int s=0;s<c.numEntries;s++)
			{
				//
				int negRun=c.t-c.cb[s]->t_last_update;
				if(c.cb[s]->stale<negRun)
					c.cb[s]->stale=negRun;
			}
			//
			/*for(n=0;n<numChannels;n++)
			{
				if(c.cb[i]->learnHigh[n]<hight[n])
					c.cb[i]->learnHigh[n]+=1;
				if(c.cb[i]->learnLow[n]>low[n])
					c.cb[i]->learnLow[n]-=1;
			}*/
			return(i);
	}
	static void find_connected_components(
		IplImage *mask,
		int poly1_hull0,
		float perimScale,
		int *num,
		CvRect *bbs,
		CvPoint *centers
		){
			static CvMemStorage *mem_storage=NULL;
			static CvSeq *contours=NULL;
			//clean up raw mask
			cvMorphologyEx(mask,mask,0,0,CV_MOP_CLOSE,CVCLOSE_ITR);
			cvMorphologyEx(mask,mask,0,0,CV_MOP_OPEN,CVCLOSE_ITR);
			//cvShowImage("close",&mask);
			//cvShowImage("open",&mask);
			//now the noise have cleaned and the find the outline of all
			if(mem_storage==NULL)
				mem_storage=cvCreateMemStorage(0);
			else 
				cvClearMemStorage(mem_storage);
			CvContourScanner scanner = cvStartFindContours(mask,mem_storage,sizeof(CvContour),
				CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE/*,cvPoint(0,0)*/);
			//once we find the all outline we need drop the small of that
			CvSeq *c;
			int numCont=0;
			while((c=cvFindNextContour(scanner))!=NULL)
			{
				double len = cvContourPerimeter(c);
				//caculate perimeter len threshold
				double q=(mask->height + mask->width)/perimScale;
				//get rid of blob if its perimeter is too small
				if (len<q)
					cvSubstituteContour(scanner ,NULL);
				else 
				{
					CvSeq *c_new;
					if(poly1_hull0)
					{
						c_new=cvApproxPoly(c,sizeof(CvContour),
							mem_storage,CV_POLY_APPROX_DP,CVCONTOUR_APPROX_LEVEL,0);
					}
					else 
					{
						c_new=cvConvexHull2(c,mem_storage,CV_CLOCKWISE,1);
					}
					cvSubstituteContour(scanner,c_new);
					numCont++;
				}
			}
			contours=cvEndFindContours(&scanner);
			//just some convenience variables
			const CvScalar CVX_WHITE = CV_RGB(0xff,0xff,0xff);
			const CvScalar CVX_BLACK = CV_RGB(0x00,0x00,0x00);

			//paint the found regions back into the image
			cvZero(mask);
			IplImage *maskTemp;
			if(num!=NULL)
			{
				int N=*num,numFilled=0,i=0;
				CvMoments moments;
				double M00,M01,M10;
				maskTemp=cvCloneImage(mask);
				for(i=0,c=contours;c!=NULL;c=c->h_next,i++)
				{
					if(i<N)
					{
						cvDrawContours(maskTemp,c,CVX_WHITE,CVX_WHITE,-1,CV_FILLED,8);
						if(centers!=NULL)
						{
							cvMoments(maskTemp,&moments,1);
							M00=cvGetSpatialMoment(&moments,0,0);
							M10=cvGetSpatialMoment(&moments,1,0);
							M01=cvGetSpatialMoment(&moments,0,1);
							centers[i].x=(int)(M10/M00);
							centers[i].y=(int)(M01/M00);
						}
						if(bbs!=NULL)
							bbs[i]=cvBoundingRect(c);
						cvZero(maskTemp);
						numFilled++;
					}
					cvDrawContours(mask,c,CVX_WHITE,CVX_WHITE,-1,CV_FILLED,8);
				}
				*num=numFilled;
				cvReleaseImage(&maskTemp);
			}
			else
			{
				for(c=contours;c!=NULL;c=c->h_next)
					cvDrawContours(mask,c,CVX_WHITE,CVX_BLACK,-1,CV_FILLED,8);
			}
	}
	//有移动前景目标的背景
	static int clear_stale_entries(codeBook &c){
		int staleThresh = c.t>>1;
		int *keep=new int [c.numEntries];
		int keepCnt = 0;
		//
		for(int i=0;i<c.numEntries;i++)
		{
			if(c.cb[i]->stale > staleThresh)
				keep[i]=0;
			else
			{
				keep[i]=1;
				keepCnt+=1;
			}
		}

		c.t=0;
		code_element **foo=new code_element *[keepCnt];
		int k=0;
		for(int ii=0;ii<c.numEntries;ii++)
		{
			if(keep[ii])
			{
				foo[k]=c.cb[ii];
				//add
				foo[k]->stale=0;
				foo[k]->t_last_update =0;
				k++;
			}
		}
		//clean up
		delete [] keep;
		delete [] c.cb;
		c.cb=foo;
		int numCleared = c.numEntries - keepCnt;
		c.numEntries=keepCnt;
		return(numCleared);
	}

	static uchar background_diff(
		uchar *p,
		codeBook &c,
		int numChannels,
		int *minMod,
		int *maxMod){
			int matchChannel;
			//
			int i;
			for(i=0;i<c.numEntries;i++)
			{
				matchChannel=0;
				for(int n=0;n<numChannels;n++)
				{
					if((c.cb[i]->min[n]-minMod[n]<=*(p+n))&&
						(*(p+n)<=c.cb[i]->max[n]+maxMod[n]))
					{
						matchChannel++;
					}
					else break;
				}
				if(matchChannel==numChannels)  break;
			}
			if(i==c.numEntries) 
			{
				return(255); 
			}
			return(0);
	}
};
#endif
