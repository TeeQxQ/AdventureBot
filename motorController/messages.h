#include "ArduinoJson.h"

/*  This is the definition (as JSON) for the message.
 *  If this definition is changed later, the following tool
 *  can be used to calculate new value for msgCapacity
 *  https://arduinojson.org/v6/assistant/
 *  
 *  {
 *   "speed": 100,  //Selected speed
 *   "left": -1,    //Direction for the left side -1, 0 (stationary), 1
 *   "right": -1    //Direction for the right side -1, 0, 1
 *  }
 *  
 */

namespace msg
{

const int msgCapacity = 96;
StaticJsonDocument<msgCapacity> msg;
DeserializationError err;

}
