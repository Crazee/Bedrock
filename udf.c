/*
   Copyright 2017 Guy Riddle

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "sqlite3ext.h"
SQLITE_EXTENSION_INIT1

/*
 * One sample SQLite stored function that is automatically loaded into each database opened.
 *
 * You might use a different cheese, of course.
 *
 * Demonstrate via:		"SELECT gouda(1.2, 2.3, 5.0, 3.4);"
 *
 */

#include <math.h>

/*
DELIMITER |
 CREATE FUNCTION GeoDistKM( lat1 FLOAT, lon1 FLOAT, lat2 FLOAT, lon2 FLOAT ) RETURNS float DETERMINISTIC
  BEGIN
  DECLARE pi, q1, q2, q3 FLOAT;
  DECLARE rads FLOAT DEFAULT 0;
  SET pi = PI();
  SET lat1 = lat1 * pi / 180;
  SET lon1 = lon1 * pi / 180;
  SET lat2 = lat2 * pi / 180;
  SET lon2 = lon2 * pi / 180;
  SET q1 = COS(lon1-lon2);
  SET q2 = COS(lat1-lat2);
  SET q3 = COS(lat1+lat2);
  SET rads = ACOS( 0.5*((1.0+q1)*q2 - (1.0-q1)*q3) );
  RETURN 6378.388 * rads;
 END;
|
DELIMITER ;
*/

	static void 
gouda_function(
	sqlite3_context						*context,
	int									argc,
	sqlite3_value						**argv
){
	double								lat1 = sqlite3_value_double(argv[0]);
	double								lon1 = sqlite3_value_double(argv[1]);
	double								lat2 = sqlite3_value_double(argv[2]);
	double								lon2 = sqlite3_value_double(argv[3]);
	double								pi, q1, q2, q3;
	double								rads, answer;

	pi = M_PI;

	lat1 = lat1 * pi / 180;
	lon1 = lon1 * pi / 180;
	lat2 = lat2 * pi / 180;
	lon2 = lon2 * pi / 180;

	q1 = cos(lon1 - lon2);
	q2 = cos(lat1 - lat2);
	q3 = cos(lat1 + lat2);

	rads = acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3));

	answer = 6378.388 * rads;

	sqlite3_result_double(context, answer);
}

	int 
load_udf_definitions(
	sqlite3								*db,
	const char							**pzErrMsg,
	const struct sqlite3_api_routines	*pApi
){
	int									rc = SQLITE_OK;
	SQLITE_EXTENSION_INIT2(pApi);

	rc = sqlite3_create_function(db, "GeoDistKM", 4, SQLITE_UTF8|SQLITE_DETERMINISTIC, 0, gouda_function, 0, 0);

	return(rc);
};

#undef sqlite3_auto_extension

	void
udf_initialize(){
	sqlite3_auto_extension((void *) &load_udf_definitions);
}
