#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "../sgp4/libsgp4/CoordTopocentric.h"
#include "../sgp4/libsgp4/CoordGeodetic.h"
#include "../sgp4/libsgp4/Observer.h"
#include "../sgp4/libsgp4/SGP4.h"

/**
 * Static values - do not change
 */
#define MOVE_UP 'u'
#define MOVE_DOWN 'd'
#define MOVE_LEFT 'l'
#define MOVE_RIGHT 'r'
#define QUERY_POSITION '?'
#define PRECISION 1.0f

/**
 * Variables that should be loaded in future from configuration or command line parameters
 */

#define MOCK 1

// Your magnetic declanation fix, for example from http://www.magnetic-declination.com/
// converted to radians. If not specified, heading is slightly off.
const float myDeclanation = +0.157f;

// Your location
const float myLat = 61.1029709f;
const float myLng = 26.1691147f;
const float myHeight = 100.0f; // meters from sea level

// Keps format data for the satellite
// Because of SGP4 library's limitations the values have to be exactly
// at right locations and the lines exactly the right length!

// Saudi-OSCAR 50
// http://www.satview.org/?sat_id=27607U
Tle tle = Tle("Saudi-OSCAR 50          ", // 24
"1 27607U 02058C   16262.37868737  .00000081  00000-0  32481-4 0  9994", // 69
"2 27607  64.5541 236.1075 0057639 195.9937 163.9347 14.75209607739010"); // 69


/*

The Keplerian Elements Short Tutorial
provided by Miguel Menendez Vazquez, EA1BCU

Numb
What are the Keplerian Elements?
The Keplerian Elements are a set of numbers which allow satellite tracking programs to calculate a satellite's position in space. The keplerian elements come in two formats - either the NASA 2-line elements (TLE) or the AMSAT verbose format elements.

Keps, as they are sometimes called, give us specific information about a satellite's orbit at a specific moment. Once these elements are known for a specific time, the satellite's position in space can be predicted using complex mathematical calculations. There is, however, one problem. Keps are given for a specific time. The accuracy of the position prediction degrades as time goes by. The predicted postion of a satellite using 7 days old keps is more accurate than a prediction using 3 months old keps.

Another factor to take into consideration is the height of the satellite's orbit. High orbiting satellites, such as Oscar 40, suffer much less from the effect of the earth's atmosphere and gravity than lower orbiting satellites, as the International Space Station and the Low Earth Orbiting (LEO) satellites. Thus the age of keps has a lower influence on predictions for high orbiting satellites than for the low orbiting ones. Also, the ISS can be maneuvered in space to avoid collisions with other objects in space, so the keps for it should be updated more frequently
We suggest updating the keps every 2 weeks, depending on the height of the satellite you're interesed in. Update every couple of weeks for low orbiting satellites, and every 4-5 weeks for high orbiting ones.

Keplerian Elements For the International Space Station
6 May, 2004

ISS
1 25544U 98067A   04127.92349537  .00017095  00000-0  14786-3 0  7232
2 25544  51.6276 176.0525 0011067 106.0444 249.6038 15.69246258311835
2-Line Element Definition (use key to decode)
1 AAAAAU YYLLLPPP  BBBBB.BBBBBBBB  .CCCCCCCC  DDDDD-D  EEEEE-E F GGGGZ
2 AAAAA  HHH.HHHH III.IIII JJJJJJJ KKK.KKKK MMM.MMMM NN.NNNNNNNNRRRRRZ

Key

[1] - Line #1 label
[2] - Line #2 label
[AAAAA] - Catalog Number assigned sequentially (5-digit integer from 1 to 99999)
[U] - Security Classification (U = Unclassified)
[YYLLLPPP] - International Designator (YY = 2-digit Launch Year; LLL = 3-digit Sequential Launch of the Year; PPP = up to 3 letter Sequential Piece ID for that launch)
[BBBBB.BBBBBBBB] - Epoch Time -- 2-digit year, followed by 3-digit sequential day of the year, followed by the time represented as the fractional portion of one day
[.CCCCCCCC] - ndot/2 Drag Parameter (rev/day2) -- one half the first time derivative of the mean motion. This drag term is used by the SGP orbit propagator.
[DDDDD-D] - n double dot/6 Drag Parameter (rev/day3) -- one sixth the second time derivative of the mean motion. The "-D" is the tens exponent (10-D). This drag term is used by the SGP orbit propagator.
[EEEEE-E] - Bstar Drag Parameter (1/Earth Radii) -- Pseudo Ballistic Coefficient. The "-E" is the tens exponent (10-E). This drag term is used by the SGP4 orbit propagator.
[F] - Ephemeris Type -- 1-digit integer (zero value uses SGP or SGP4 as provided in the Project Spacetrack report.
[GGGG] - Element Set Number assigned sequentially (up to a 4-digit integer from 1 to 9999). This number recycles back to "1" on the update following element set number "9999."
[HHH.HHHH] - Orbital Inclination (from 0 to 180 degrees).
[III.IIII] - Right Ascension of the Ascending Node (from 0 to 360 degrees).
[JJJJJJJ] - Orbital Eccentricity -- there is an implied leading decimal point (between 0.0 and 1.0).
[KKK.KKKK] - Argument of Perigee (from 0 to 360 degrees).
[MMM.MMMM] - Mean Anomaly (from 0 to 360 degrees).
[NN.NNNNNNNN] - Mean Motion (revolutions per day).
[RRRRR] - Revolution Number (up to a 5-digit integer from 1 to 99999). This number recycles following revolution nymber 99999.
[Z] - Check Sum (1-digit integer). Both lines have a check sum that is computed from the sum of all integer characters on that line plus a "1" for each negative (-) sign on that line. The check sum is the modulo-10 (or ones digit) of the sum of the digits and negative signs.

*/

/**
 * Function to get index of character in char* string
 */
/*int indexOf(const char *str, char matchChr)
{
  for(int i=0;;i++)
  {
    if (str[i] == matchChr) return i;
    if (str[i] == 0) return -1;
  }
}*/

bool getPosition(float *azi, float *ele)
{
#ifdef MOCK
  *azi = 160;
  *ele = 10;
  return true;
#endif
}

bool movePosition(char cmd)
{
  std::cout << "Movement: " << cmd << std::endl;
#ifdef MOCK
#endif
}

int main(void)
{
  float myAzimuth, myElevation;

  // Initialize SGP4 library with our position and satellite KEPS data
  Observer obs(myLat, myLng, myHeight / 1000.0f);
  SGP4 sgp4(tle);

  std::cout << tle << std::endl;

  while (true)
  {
    // Get satellite position and our direction to it
#ifdef MOCK
    DateTime now = DateTime(2016, 9, 18, 15, 50, 00);
#else
    DateTime now = DateTime::Now(true);
#endif
    Eci eci = sgp4.FindPosition(now);
    CoordTopocentric topo = obs.GetLookAngle(eci);
    CoordGeodetic geo = eci.ToGeodetic();

    std::cout << topo << geo << std::endl;

    // Check if the satellite should be visible on sky
    if (topo.elevation < 0)
    {
      std::cout << "Satellite is below horizon." << std::endl;
      sleep(1);
    }
    else if (topo.elevation > 90)
    {
      std::cout << "Satellite elevation is over +90 degrees - error?" << std::endl;
      sleep(1);
    }
    else
    {
      for (int i=0; i<10; i++)
      {
        // Check real position of the antenna
        if (getPosition(&myAzimuth, &myElevation))
        {
          std::cout << "My azimuth and elevation: " << myAzimuth << ", " << myElevation << std::endl;
          //std::cout << "Sat azimuth and elevation: " << topo.azimuth << ", " << topo.elevation << std::endl;
          // Calculate differences from position to wanted position
          float dAzimuth = myAzimuth - Util::RadiansToDegrees(topo.azimuth);
          float dElevation = myElevation - Util::RadiansToDegrees(topo.elevation);
          //std::cout << "dAzimuth: " << dAzimuth << " dElevation: " << dElevation << std::endl;
          // Control antenna if it's off too much
          if (abs(dAzimuth) > PRECISION)
          {
            if (dAzimuth < 0) 
              movePosition(MOVE_RIGHT); 
            else 
              movePosition(MOVE_LEFT);
          }
          if (abs(dElevation) > PRECISION)
          {
            if (dElevation < 0) 
              movePosition(MOVE_UP); 
            else 
              movePosition(MOVE_DOWN);
          }
        }
        usleep(100000);
      }
    }
  };

  return 0;
}
