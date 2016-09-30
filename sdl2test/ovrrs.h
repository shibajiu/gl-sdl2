#pragma once
#include <pxcsensemanager.h>
#include <pxctouchlesscontroller.h>
#include <pxchandmodule.h>
#include <pxchandconfiguration.h>
#include <gtx/transform.hpp>
#include "Tree.h"
#include <vector>
#define IFCERR(b,msg) if(b){cerr<<msg<<endl;return;} 
#define PXCIFERR(b,msg) IFCERR(b<pxcStatus::PXC_STATUS_NO_ERROR,msg)
#define MAX_NUMBER_OF_JOINTS 22
#define MAX_NUMBER_OF_HANDS 2


typedef PXCTouchlessController::AlertData::AlertType ovrAttp;
class ovrUXEventHandler;
class ovrAlertHandler;
class ovrHandAlertHandler;
class HandsModel;

class ovrrs_tc {
private:
	friend class ovrUXEventHandler;
	friend class ovrAlertHandler;
	PXCSession *ps;
	PXCSenseManager *psm;
	PXCTouchlessController *ptc;
	ovrUXEventHandler *handlerUXEvent;
	ovrAlertHandler * handlerAlert;
	glm::mat4 roll;
	glm::vec3 oldpos;
	glm::vec3 pos;
	glm::vec3 moved;

	bool isStop;
	ovrAttp alert;

	void SetConfiguration();
	void SetAlert(ovrAttp);

public:
	//using GL::setWindowTitle(GLFWwindow*, bool);
	ovrrs_tc();
	~ovrrs_tc();
	void Start();
	glm::vec3 getHandMove();
	glm::vec3 getHandPos();
	void Stop();
	ovrAttp GetAlert();
};

class ovrUXEventHandler :public PXCTouchlessController::UXEventHandler {
private:
	friend class ovrrs_tc;
	ovrrs_tc* rs=nullptr;
	glm::vec3 PXCPoint3_to_vec3(const PXCPoint3DF32 &);
	glm::vec3 getPosition_from_UXEventData(PXCTouchlessController::UXEventData *);

public:
	virtual void PXCAPI OnFiredUXEvent(const PXCTouchlessController::UXEventData *);
};

class ovrAlertHandler :public PXCTouchlessController::AlertHandler {
private:
	friend class ovrrs_tc;
	ovrrs_tc* rs=nullptr;
public:
	virtual void PXCAPI OnFiredAlert(const PXCTouchlessController::AlertData *);
};

class ovrActionHandler :public PXCTouchlessController::ActionHandler {
public:
	virtual void PXCAPI OnFiredAction(const PXCTouchlessController::Action);
};

class ovrrs_fh {
private:
	ovrHandAlertHandler *alert;
	vec3 orientation;
	bool isStop;
	HandsModel* handsmodel;
	
	void init();
	vec3 PXCPoint3DF32_to_vec3(PXCPoint3DF32 _p) const;
	quat PXCPoint4DF32_to_quat(PXCPoint4DF32 _p) const;

public:
	int Start();
	int Stop();
	vec3 GetWristOrientation();
	vec3* GetJointPoints();
	void Release();
	~ovrrs_fh();
};

class ovrHandAlertHandler :public PXCHandConfiguration::AlertHandler {
public:
	virtual void PXCAPI OnFiredAlert(const PXCHandData::AlertData &_d);
};

class HandsModel {
private:
	friend class ovrrs_fh;
	Tree<PointData>* skeletontree;
	PXCHandData* handdata;
	bool righthand = false, lefthand = false;
	//vec3* jointpoints=new vec3[MAX_NUMBER_OF_JOINTS];
	vec3 *jointpoints_t= new vec3[MAX_NUMBER_OF_JOINTS];;

	void copyJointToPoint(PointData & dst, const PXCHandData::JointData & src);
	//void copyJointTopoint()

public:
	HandsModel(PXCHandData*);
	~HandsModel();
	void updateskeletonTree();
	vec3 PXCPoint3DF32_to_vec3(PXCPoint3DF32 _p) const;
	vec3* GetPoint();
	void Release();
};