﻿#pragma once
#include <Arduino.h>
#include "constants.h"

const double Rad=57.29577951;

enum Catalog { STAR, MESSIER, CALDWELL, HERSCHEL, CAT_NONE };
const int FM_NONE = 0;
const int FM_ABOVE_HORIZON = 1;
const int FM_ALIGN_ALL_SKY = 2;
const int FM_CONSTELLATION = 4;
const int FM_OBJ_TYPE      = 8;

void getcatdms(const short& v, short& v1, uint8_t& v2);
void getcatdf(const short& v, float& v1);
void getcathms(const unsigned short& v, uint8_t& v1, uint8_t& v2, uint8_t& v3);
void getcathf(const unsigned short& v, float& v1);

class CatMgr {
  public:
// initialization
    void setLat(double lat);
    void setLstT0(double lstT0);
    bool isInitialized();

// time
    double lstDegs();
    double lstHours();

// catalog selection
    void select(Catalog cat);
    Catalog getCat();
    const char* catalogStr();

// catalog filtering
    void filtersClear();
    void filterAdd(int fm);
    void filterAdd(int fm, int param);

// select catalog record
    bool setIndex(int index);
    int getIndex();
    int getMaxIndex();
    bool incIndex();
    bool decIndex();

// get catalog contents
    double ra();
    double rah();
    double ha();
    void   raHMS(uint8_t& h, uint8_t& m, uint8_t& s);
    double dec();
    void   decDMS(short& d, uint8_t& m, uint8_t& s);
    int    epoch();
    double alt();
    double azm();
    void topocentricToObservedPlace(float *RA, float *Dec);

    float magnitude();
    
    byte   constellation();
    const char* constellationStr();
    
    byte   objectType();
    const char* objectTypeStr();
    const char* objectName();
    
    int    primaryId();

private:
    double _lat=-10000;
    double _cosLat=0;
    double _sinLat=0;
    double _lstT0=0;
    unsigned long _lstMillisT0=0;
    Catalog _cat=CAT_NONE;
    
    int _fm=FM_NONE;
    int _fm_con=0;
    int _fm_obj_type=0;
    
    int _selected=0;
    int _idx[5]={0,0,0,0,0};
    int _maxIdx[5]={
	    NUM_STARS-1,
	    NUM_MESSIER-1,
	    NUM_CALDWELL-1,
      NUM_HERSCHEL-1,
	    0-1};

    bool isFiltered();

    double DistFromEqu(double RA, double Dec);
    double HAToRA(double ha);
    void EquToHor(double RA, double Dec, double *Alt, double *Azm);
    void HorToEqu(double Alt, double Azm, double *RA, double *Dec);
    double TrueRefrac(double Alt, double Pressure=1010.0, double Temperature=10.0);

    double cot(double n);
};

extern CatMgr cat_mgr;
