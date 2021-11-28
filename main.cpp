#include <locale.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include<algorithm>
 
using namespace std;

/*
 AUTOR : Edermar Dominguez
 CI: 23.689.009
*/

// Declaracion de constantes usadas en el proyecto 

// Separadores del JSON
const string SEPARATOR = "}";  
const string LIMIT = ",\"";  
const string LIMIT_TWO = "\",\""; 
const string COMMA = ","; 
const string WHITE_SPACE = " "; 

// Utils
const int VALUE_TRUE       		   = 1;
const int QUANTITY_OF_OBJECTS_PER_LIST = 4; 
const string EMPTY = ""; 

// Parametros JSON
const string OBJECT_RECORD		 = "acta";  
const string VALUE_RECORD_YEAR   = "año";  
const string VALUE_RECORD_NUMBER = "número";  

const string OBJECT_AUTHOR 			 = "autor";  
const string VALUE_AUTHOR_NAME 		 = "nombre";  
const string VALUE_AUTHOR_DOCUMENT   = "cédula";  
const string VALUE_AUTHOR_MAIL		 = "email";  

const string VALUE_DATE		 = "fecha";  
const string VALUE_TITLE     = "titulo";  
const string VALUE_MODE		 = "modalidad";  

const string OBJECT_JUDGE		 = "jurado";  
const string VALUE_JUDGE_TUTOR   = "tutor";  
const string VALUE_JUDGE_ONE     = "jurado1";  
const string VALUE_JUDGE_TWO     = "jurado2";  

const string OBJECT_NOTE		     = "nota";  
const string VALUE_NOTE_NO_NUMERIC   = "no_numérica";  
const string VALUE_NOTE_NUMERIC      = "numerica";  

// Tipos de Modalidad

const string VALUE_MODE_INVESTIGATION	= "Investigación";  
const string VALUE_MODE_INTERNSHIP   	= "Pasantía"; 
const string VALUE_MODE_CEG		 		= "CEG"; 
const string VALUE_MODE_INVESTIGATION_WITHOUT_ACCENT	= "Investigacion";  
const string VALUE_MODE_INTERNSHIP_WITHOUT_INTERSHIP   	= "Pasantia"; 

// Tipos de Nota no numerica

const string VALUE_NOTA_NO_NUMERIC_HONOR		 	= "honorifica"; 
const string VALUE_NOTA_NO_NUMERIC_APPROVE	 		= "aprobado"; 
const string VALUE_NOTA_NO_NUMERIC_REPROVE	 		= "reprobado"; 

// Codigo de errores

const string ERROR_WRONG_MODALITY		 	    = "CODE_100"; 
const string ERROR_WRONG_NO_NUMERIC_NOTE 		= "CODE_200"; 
const string ERROR_WRONG_NUMERIC_NOTE 			= "CODE_300"; 
const string ERROR_WRONG_NUMERIC_NOTE_DECIMALS  = "CODE_350"; 

const int ERROR_WRONG_DATE_IN_PARAMS		= 400; 
const int ERROR_WRONG_DATE_IN_DAY			= 410; 
const int ERROR_WRONG_DATE_IN_MONTH			= 420; 
const int ERROR_WRONG_DATE_IN_YEAR			= 430; 
const int ERROR_WRONG_DATE_IN_DELIMITER_ONE = 440; 
const int ERROR_WRONG_DATE_IN_DELIMITER_TWO	= 450; 
const int ERROR_WRONG_DATE_INVALID       	= 460;
 
// Mensajes : 

const string ERROR_INVALID_ROUTE  = "Ruta Inexistente"; 
const string ERROR_INVALID_JSON   = "Json erroneo"; 
const string ERROR_INVALID_OBJECT = "Error: Esta estructura posee inconsistencias en los objetos { }"; 
const string ERROR_INVALID_ARRAY  = "Error: Esta estructura posee inconsistencias en las llaves [ ]"; 
const string FILE_OK              = "Archivo Ok"; 
const string WRITE_FILE_PATH      = "Escriba la direccion donde se encuentra el archivo JSON a parsear"; 
const string ERROR_MISSING_FIELD_JSON   = "Error: Faltan campos en el json"; 


// Una forma mas corta de imprimir cosas en consola

void print(string data) {
	cout << data << endl;
}

void print(int data) {
	cout << data  << endl;
}

/* 
	Metodo encargado de verificar si la estructura de entrada es un JSONArray
	para parsear los objetos de manera distinta
	@Params
    isArray: booleano a returnar
*/
boolean isJsonArray(string json) {
	char firstCharacter= json[0];
	char lastCharacter= json[json.length()-1];
	boolean isArray = false;
	
	if(firstCharacter == '['){
		if(lastCharacter == ']'){
	  		isArray = true;
		}
	}
	
	return isArray;
}

/* 
	Metodo encargado de pedirle al usuario la ruta de acceso para buscar el JSON
	y devolverla en una variable para su uso futuro, devolvera la ruta si esta posee
	un archivo json valido en caso contrario devuelve null
*/
string getFilePath() {
	string filePath;
	print(WRITE_FILE_PATH);
	cin >> filePath;
	
	ifstream f(filePath.c_str());
    if(f.good()) {	
    	if(filePath.find(".json")){
    		return filePath;
		}else{
			return EMPTY;
		}
	}else{
		return EMPTY;
	}
}

/* 
	Metodo encargado de tomar la ruta base y limpiarala para 
	crear el archivo de salida en el mismo sitio que el de entrada
   
  @Params
  *filename: Direccion del archivo base
*/
string getBasePath(string filename) {
	const size_t last_slash_idx = filename.find_last_of("\\/");
	const size_t period_idx = filename.rfind('.');
	
	if (std::string::npos != period_idx)
	{
	    filename.erase(period_idx);
	}
	
	return filename;
}

/* 
   Metodo encargado remover los caracteres especiales que tiene un parametro
   para retornar su valor sin caracteres especiales.
    
   @Params
   data: string a limpiar
*/
string cleanString(string data) {
	data.erase(remove(data.begin(), data.end(), '\"'), data.end());
	data.erase(remove(data.begin(), data.end(), '}'), data.end());
	return data;
}

/* 
   Metodo encargado de verificar si la modalidad cumple con alguno de los 3 formatos solicitados,
   en caso contrario devuelve un codigo de error
    
   @Params
   modality: string con el parametro a verificar
*/
string checkModality(string modality) {
	if(modality == VALUE_MODE_INVESTIGATION || modality == VALUE_MODE_INVESTIGATION_WITHOUT_ACCENT || modality == VALUE_MODE_INTERNSHIP || modality == VALUE_MODE_INTERNSHIP_WITHOUT_INTERSHIP || modality == VALUE_MODE_CEG ) {
		return modality;
	}else{
		return ERROR_WRONG_MODALITY;
	}
}

/* 
   Metodo encargado de verificar si la nota no numerica cumple con alguno de los 3 formatos solicitados,
   en caso contrario devuelve un codigo de error
    
   @Params
   note: string con el parametro a verificar
*/
string checkNoNumericNote(string note) {
	if(note == VALUE_NOTA_NO_NUMERIC_HONOR || note == VALUE_NOTA_NO_NUMERIC_APPROVE || note == VALUE_NOTA_NO_NUMERIC_REPROVE ) {
		return note;
	}else{
		return ERROR_WRONG_NO_NUMERIC_NOTE;
	}
}

/* 
   Metodo encargado de verificar si la nota numerica cumple con alguno de los formatos solicitados,
   en caso contrario devuelve un codigo de error, primero verifica que el valor no tenga una parte decimal
   y luego verifica que este dentro del rango aceptado
    
   @Params
   note: string con el parametro a verificar
*/
string checkNumericNote(string note) {
	int havePoint = note.find(".");
	int haveComma = note.find(",");
	
	if(havePoint != -1 || haveComma != -1) {
		return ERROR_WRONG_NUMERIC_NOTE_DECIMALS;
	}else{
		int noteInt = atoi(note.c_str());			
		if(noteInt >= 0 && noteInt <= 10){
			return note;
		}else{
			return ERROR_WRONG_NUMERIC_NOTE;
		}
    }
}

/*
	Metodo encargado de verificar manualmente si el caracter enviado por parametro
	pertene a un numero entero
	@Params
	number: contiene un caracter con el numero a verificar
*/
boolean isNumber(string number) {
	string aNumbers[10] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}; 

	for(int i=0; i<=9; i++) {
		if(number == aNumbers[i]) {
			return true;
		}
	}
	return false;
}

/*
	Metodo encargado de hacer una revision completa a la fecha, valida, año bisiesto
	y meses que incluyen dia 30 y 31
	@Params
	day: Dia
	month: mes
	year: año
*/
bool isValidDate(int day, int month, int year)
{
	if(year < 0){
		return false;
	}
	
		
	if(month < 0 || month > 12){
		return false;
	}
	
	if(month == 2) {	
		if (year % 4 == 0) 
		{
			if( day > 29 || day <= 0)
			{
				return false;
			}
		}
		else
		{
			if( day > 28 || day <= 0)
			 {
				return false;
			}
		}
	}
	else if(month == 1 || month == 3 || month  == 5 || month == 7 || month == 8 || month == 10 || month == 12)
	{
		if(day > 31 || day <= 0) {
			return false;
		}
	}
	else
	{
		if(day > 30 || day <= 0){
			return false;
		}
	}
	return true;
}

/*
	Metodo encargado de parsear detalladamente todos los datos de la fecha y sus delimitadores
	devolviendo un error personalizado por cada uno de los errores que se encuentren.
	@Params
	date: Fecha en formato dd/mm/yy
*/
int checkDate(string date) {
	if(date.length() == 8) {
		if(isNumber(date.substr(0, 1)) && isNumber(date.substr(1, 1))) {
			if(date.substr(2, 1) == "/") {
				if(isNumber(date.substr(3, 1)) && isNumber(date.substr(4, 1))){
					if(date.substr(5, 1) == "/") {
						if(isNumber(date.substr(6, 1)) && isNumber(date.substr(7, 1))){
							int day   = atoi(date.substr(0, 2).c_str());
							int month = atoi(date.substr(2, 2).c_str());
							int year  = atoi(date.substr(6, 2).c_str());		
						    
							if(isValidDate(day, month, year)) {
								return VALUE_TRUE;
							}else{
								return ERROR_WRONG_DATE_INVALID;
							}
						}else{
							return ERROR_WRONG_DATE_IN_YEAR;
						}
					}else{
						return ERROR_WRONG_DATE_IN_DELIMITER_TWO;
					}
				}else{
					return ERROR_WRONG_DATE_IN_MONTH;
				}
			}else{
				return ERROR_WRONG_DATE_IN_DELIMITER_ONE;
			}
		}else{
			return ERROR_WRONG_DATE_IN_DAY;
		}
	}else{
		return ERROR_WRONG_DATE_IN_PARAMS;
	}
}

/* 
   Metodo encargado de verificar si algunos campos cumplen con las condiciones especiales
   que se exigen, devolviendo un booleano para confirmar si fue o no exitosa la revision
    
   @Params
   *aParameters: array de strings que posee las key a validar
*/
boolean isDataRight(string *aParameters) {
	boolean isDataSuccess = true;
	
	if(checkModality(aParameters[7]) == ERROR_WRONG_MODALITY) {
		isDataSuccess = false;
		print("ERROR: LA modalidad ingresada no cumple con el criterio de aceptacion, valor: "+ aParameters[7]);
	}
	
	if(aParameters[11] != WHITE_SPACE) {
		if(checkNoNumericNote(aParameters[11]) == ERROR_WRONG_NO_NUMERIC_NOTE) {
			isDataSuccess = false;
			print("ERROR: LA nota no numerica ingresada no cumple con el criterio de aceptacion, valor: "+ aParameters[11]);
		}
	}
	
	if(checkNumericNote(aParameters[12]) == ERROR_WRONG_NUMERIC_NOTE) {
		isDataSuccess = false;
		print("ERROR: La nota numerica ingresada no cumple con el criterio de aceptacion: Esta fuera de rango, valor: "+ aParameters[12]);
	}
	
	if(checkNumericNote(aParameters[12]) == ERROR_WRONG_NUMERIC_NOTE_DECIMALS) {
		isDataSuccess = false;
		print("ERROR: La nota numerica ingresada no cumple con el criterio de aceptacion: Posee decimales, valor: "+ aParameters[12]);
	}
	
	switch(checkDate(aParameters[5])) {
	    case ERROR_WRONG_DATE_IN_PARAMS:
	      print("ERROR: Fecha corrupta, valor: "+ aParameters[5]);
	      isDataSuccess = false;
	      break;
	    case ERROR_WRONG_DATE_IN_DAY:
	      print("ERROR: Dia de la fecha corrupto, valor: "+ aParameters[5]);
	      isDataSuccess = false;
	      break; 
		case ERROR_WRONG_DATE_IN_MONTH:
	      print("ERROR: Mes de la fecha corrupto, valor: "+ aParameters[5]);
	      isDataSuccess = false;
	      break; 
	    case ERROR_WRONG_DATE_IN_YEAR:
	      print("ERROR: Año de la fecha corrupto, valor: "+ aParameters[5]);
	      isDataSuccess = false;
	      break; 
	    case ERROR_WRONG_DATE_IN_DELIMITER_ONE:
	      print("ERROR: Primer delimitador de la fecha corrupto, valor: "+ aParameters[5]);
	      isDataSuccess = false;
	      break; 
	    case ERROR_WRONG_DATE_IN_DELIMITER_TWO:
	      print("ERROR: Segundo delimitador de la fecha corrupto, valor: "+ aParameters[5]);
	      isDataSuccess = false;
	      break; 
	    case ERROR_WRONG_DATE_INVALID:
	      print("ERROR: Fecha invalida, valor: "+ aParameters[5]);
	      isDataSuccess = false;
	      break; 
	}
	
	return isDataSuccess;
}

/* 
   Metodo encargado de formatear los caracteres especiales que puede contener una
   letra al momento de leer desde el archivo formateandolos a un valor adecuado.
   
   @Params
   json: json a parsear 
*/
string formatData(string json) {
	
	string jsonFormat;
	for (int i = 0; i < json.length(); i++) {
		char val = json[i];
  		if(val == 'Ã') {
  			switch(json[i+1]) {
			    case '¡'  :
			      jsonFormat = json.replace(i,2, "á"); 
			      break;
			    case ''  :
			      jsonFormat = json.replace(i,2, "Á"); 
			    break;
			    case '©'  :
			      jsonFormat = json.replace(i,2, "é"); 
			    break; 
			    case '‰'  :
			      jsonFormat = json.replace(i,2, "É"); 
			    break; 
				case '­'  :
			      jsonFormat = json.replace(i,2, "í"); 
			    break; 
			    case ''  :
			      jsonFormat = json.replace(i,2, "Í"); 
			    break; 
			    case '³'  :
			      jsonFormat = json.replace(i,2, "ó"); 
			    break; 
			    case '“'  :
			      jsonFormat = json.replace(i,2, "Ó"); 
			    break; 
			    case 'º'  :
			      jsonFormat = json.replace(i,2, "ú"); 
			    break; 
			    case 'š'  :
			      jsonFormat = json.replace(i,2, "Ú"); 
			    break;
			    case '±'  :
			      jsonFormat = json.replace(i,2, "ñ"); 
			    break; 
			}
  		}
	}
	return jsonFormat;
}

/* 
	Metodo encargado de leer el archivo de entrada linea a linea 
    para retornarlo en una variable una vez complete la lectura
    
    @Params
    path: Ruta donde se encuentra el archivo a buscar
*/
string readJSONFile(string path) {
  string jsonString;
  char cadena[254];
  ifstream file(path.c_str());
  while (!file.eof()) {
    file >> cadena;
    jsonString = jsonString + cadena;
  }
  file.close();
  return jsonString;
}

/*
	Metodo encargado de encontrar la posicion de los delimitadores "}"
	en el json para asi saber cual es el final del objeto
	
	@Params
	json: json a parsear 
*/
int * getOcurrecePositions(string json) {

	size_t ocurrence;
  	int firstOcurrence = 0;
  	static int aOcurrences[100];
  	
	for (int i = 0; i < 100; i++) {
		ocurrence = json.find(SEPARATOR, ocurrence + i);
		if (ocurrence != string::npos) {
			if(firstOcurrence == 0) {
				firstOcurrence = ocurrence;
			}else{
				if(firstOcurrence == ocurrence) {
					return aOcurrences;
				}
			}
			aOcurrences[i] = ocurrence;
		}
	}
	
	return aOcurrences;
}

/*
  Metodo encargado de tomar un string(JSON) y encontrar el Object 
  solicitado por parametro, devolviendo solo ese fragmento de texto 
  como un objeto

  @Params
  json: json a parsear
  key: Nombre del parametro a buscar
  occurence: posicion en la que se encuentra el puntero	
 */
string getObject(string json, string key, int occurence) {
	int positionStart = json.find(key);
	return json.substr(positionStart, occurence-positionStart);
}

/*
  Metodo encargado de encontrar el parametro especifico enviado en el Objeto
  formatea el string limpiando los caracteres especiales y luego lo retorna. 
  El metodo encuentra los delimitadores de cada parametro y de esta forma los 
  separa.

  @Params
  Object: JsonObject a parsear
  key: Nombre del parametro a buscar	
 */
string getKey(string object, string key) {	
	string query;
	string dirtyQuery;
	
	int positionStart = object.find(key);
	int positionEnd = object.find(LIMIT);
	dirtyQuery = object.substr(positionStart, positionEnd-positionStart);
	
	int positionStartDelimiter = dirtyQuery.find(key);
	int positionEndDelimiter = dirtyQuery.find(LIMIT_TWO);
	dirtyQuery = dirtyQuery.substr(positionStartDelimiter, positionEndDelimiter-positionStartDelimiter);

	query = cleanString(dirtyQuery);
	
	int keyPosition = query.find(key);
	query = query.substr(keyPosition + key.length() + 1);
	
	return query;
}

/* 
	Metodo encargado de obtener cada uno de los parametros y guardarlos en un array 
   temporal, el mismo obtendra todos los valores del JSON en en el formato requerido
   
  @Params
  *aOcurrences : Array con todas las posiciones de los limitadores que hacen posible el parseo
  json: String con la data a parsear
*/

string *getArrayOfParameters(int *aOcurrences, string json, boolean isFirstObject) {
	
	static string aParameters[13];
	
	for (int i = 0; i <= QUANTITY_OF_OBJECTS_PER_LIST; i++) {
		
		string object;
		int ocurrencePosition;
		
		if(isFirstObject){
			ocurrencePosition =	aOcurrences[i+1];
		}else{
			ocurrencePosition =	aOcurrences[i];
		}
		
		switch(i) 
		{
		    case 0:
		      object = getObject(json, OBJECT_RECORD, ocurrencePosition);
		      aParameters[0] = getKey(object, VALUE_RECORD_YEAR);
		      aParameters[1] = getKey(object, VALUE_RECORD_NUMBER);
		      break;
		    case 1:
		      object = getObject(json, OBJECT_AUTHOR, ocurrencePosition);
		      aParameters[2] = getKey(object, VALUE_AUTHOR_NAME);
		      aParameters[3] = getKey(object, VALUE_AUTHOR_DOCUMENT);
		      aParameters[4] = getKey(object, VALUE_AUTHOR_MAIL);
		      break; 
		    case 2:
		      object = getObject(json, OBJECT_JUDGE, ocurrencePosition);
		      aParameters[8] = getKey(object, VALUE_JUDGE_TUTOR);	
		      aParameters[9] = getKey(object, VALUE_JUDGE_ONE);	
		      aParameters[10] = getKey(object, VALUE_JUDGE_TWO);
		      break; 
		    case 3:
		      object = getObject(json, VALUE_DATE, ocurrencePosition);
		      aParameters[5] = getKey(object, VALUE_DATE);	
		      object = getObject(json, VALUE_TITLE, ocurrencePosition);
		      aParameters[6] = getKey(object, VALUE_TITLE);
		      object = getObject(json, VALUE_MODE, ocurrencePosition);
		      aParameters[7] = getKey(object, VALUE_MODE);		      	      
		      break; 
		     case 4:	
		      object = getObject(json, OBJECT_NOTE, ocurrencePosition);
			  
			  try{
			     aParameters[11] = getKey(object, VALUE_NOTE_NO_NUMERIC);
			  }catch (...) {
				 aParameters[11] = WHITE_SPACE;
			  }
			  
			  try{
		      	 aParameters[12] = getKey(object, VALUE_NOTE_NUMERIC);	
			  }catch (...) {
				 aParameters[12] = WHITE_SPACE;
			  }
		      break; 
		}
	}
	return aParameters;
}

/* 
	Metodo encargado de tomar todos los parametros del array y crear un string limpio
	que sera enviado al excel para ser guardado
   
  @Params
  *aParameters : Array con todas los valores ordenados en un fomarto RAW 
*/
string getDataToWriteInExcel(string *aParameters) {
	
	string data = aParameters[0] + WHITE_SPACE + aParameters[1] +COMMA + WHITE_SPACE + aParameters[2] + WHITE_SPACE +
				  aParameters[3] + WHITE_SPACE + "<" + aParameters[4] + ">"+ COMMA + WHITE_SPACE + aParameters[5] + COMMA + WHITE_SPACE +
	 			  aParameters[6] + COMMA +  WHITE_SPACE + aParameters[7] + COMMA +  WHITE_SPACE + aParameters[8] + WHITE_SPACE + aParameters[9] +
	  			  WHITE_SPACE + aParameters[10] + COMMA + WHITE_SPACE + aParameters[11] + WHITE_SPACE + aParameters[12]; 
	return data;		
}

/* 
	Metodo encargado de crear un archivo csv y escribir en el todos los parametros en el
	orden que requerido para hacer el input del proyecto.
   
  @Params
  filePath: Direccion donde se creara el archivo de salida
  *aInfoToSave : Array con todas los valores de los parametros JSON formateados
  y preparados para ser guardados 
  aInfoToSaveLenght: int que maneja la longitud del array
*/
void createCSV(string filePath, string *aInfoToSave, int aInfoToSaveLenght) {
    string path = getBasePath(filePath) + "Formateada.csv";
	std::ofstream file(path.c_str());
	for (int i = 0; i <= aInfoToSaveLenght-1; i++) {
		file << aInfoToSave[i];
		file << '\n';
		file << '\n';
	}
	
    file.close();
    
    print("Archivo OK, salida CSV creada en "+ path);
}

/* 
	Metodo encargado de verificar la estructura de los objetos y las llaves
	en caso de que no coincidan devolvera un error
   
  @Params
  json : string con la data a parsear 
*/
string checkStructureOfJSON(string json) {
	
	int quantityOfObjectDelimiterStart = 0;
	int quantityOfObjectDelimiterEnd = 0;
	int quantityOfArrayDelimiterStart = 0;
	int quantityOfArrayDelimiterEnd = 0;
	string error = EMPTY;
			
	for (int i = 0; i < json.length(); i++) {
		char aux = json[i];
	  	
	  	if(aux == '{') {
  			quantityOfObjectDelimiterStart++;
		}	
		
		if(aux == '}') {
			quantityOfObjectDelimiterEnd++;
		}
		
		if(aux == '[') {
			quantityOfArrayDelimiterStart++;
		}
		
		if(aux == ']') {
			quantityOfArrayDelimiterEnd++;
		}
	}
	
	if(quantityOfObjectDelimiterStart != quantityOfObjectDelimiterEnd) {
		error = ERROR_INVALID_OBJECT;
	}
	
	if(quantityOfArrayDelimiterStart != quantityOfArrayDelimiterEnd) {
		error = ERROR_INVALID_ARRAY;
	}

	return error;
}

/* 
	Este flujo se inicia al momento de tener un JSONObject definido
	tomara los parametros y se encargara de enviarlos a otro sitio para
	procesar la data en excel
   
  @Params
  filePath : direccion donde se encuentra el archivo de entrada
  json : string con la data a parsear 
  aObjectsLenght: int que indica la cantidad de items del array json para recorrerlo
*/
void getDataOfJsonObjectFlow(string filePath, string *json, int aObjectsLenght) {
	string aInfoToSave[aObjectsLenght];
	int aInfoToSaveLenght = 0;
	string *aParameters;
	
	for (int i = 0; i <= aObjectsLenght-1; i++) {
		
		if(i == 0){
			aParameters = getArrayOfParameters(getOcurrecePositions(json[i]), json[i], true);
		}else{
			aParameters = getArrayOfParameters(getOcurrecePositions(json[i]), json[i], false);
		}
			
		if(isDataRight(aParameters)) {
		 	aInfoToSave[i] = getDataToWriteInExcel(aParameters);
		 	aInfoToSaveLenght++;
		}else{
		 	print(ERROR_INVALID_JSON);
		 	return;
		}
		
	}
	createCSV(filePath, aInfoToSave, aInfoToSaveLenght);
}

/* 
	Este flujo se inicia al momento de tener un JSONARRAY el mismo 
	recorre todo el string y separa el array en objetos que seran 
	utlizados luego
   
  @Params
  filePath : direccion donde se encuentra el archivo de entrada
  json : string con la data a parsear 
*/
void getDataOfJsonArrayFlow(string filePath, string json) {

	int quantityOfDelimitiersOpen = 0;
	int quantityOfDelimitiersClosed = 0;
	int positionObjectInitial = 0;
	int positionObjectFinal = 0;
	int aObjectsLenght = 0;
	string aObjects[100];
	
	for (int i = 0; i < json.length(); i++) {
		char aux = json[i];
	  	
	  	if(aux == '{') {
	  		if(positionObjectInitial == 0){
	  			positionObjectInitial = i;	
			}
  			quantityOfDelimitiersOpen++;
		}	
		
		if(aux == '}') {
			quantityOfDelimitiersClosed++;
		}
		
		if(quantityOfDelimitiersOpen == 5 && quantityOfDelimitiersClosed == 5) {
			positionObjectFinal = i;
		
			string object = json.substr (positionObjectInitial, positionObjectFinal - positionObjectInitial); 
			aObjects[aObjectsLenght] = object;
			quantityOfDelimitiersOpen = 0;
			quantityOfDelimitiersClosed = 0;
			positionObjectInitial = 0;
			positionObjectFinal = 0;
			aObjectsLenght++;
		}
	}	
	
	if(aObjectsLenght > 0) {
		getDataOfJsonObjectFlow(filePath, aObjects, aObjectsLenght);	
	}
	
}

/* 
	Metodo encargado de verificar la estructura de los objetos y las llaves
	en caso de que no coincidan devolvera un error
   
  @Params
  json : string con la data a parsear 
  filePath: Direccion donde se leera el archivo de entrada
*/
void initFlowParseJson(string filePath, string json) {
	try {
		json = readJSONFile(filePath);
		json = formatData(json);
		
		string jsonErrors = checkStructureOfJSON(json);
		
		if(jsonErrors == "") {	
		
			if(isJsonArray(json)){
				getDataOfJsonArrayFlow(filePath, json);
			}else{
				string aux[1];
				aux[0] = json;
				getDataOfJsonObjectFlow(filePath, aux, 1);
			}
					
		}else{
			print(jsonErrors);
		}
	}
	catch (...) {
	  print(ERROR_MISSING_FIELD_JSON);
	}
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "");
	string json;
	string filePath;
	
	filePath = getFilePath();
	
	if(filePath == EMPTY){
		print(ERROR_INVALID_ROUTE);
	}else{
		json = readJSONFile(filePath);
		initFlowParseJson(filePath, json);
	}
	
	system("pause");
}

/*

TODO: 

- especificacion ebnf 

- soporte para varios registros

Errores:

- Elementos en desorde hay error.
 
 */

