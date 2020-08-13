#pragma once

#include <fstream>
#include <string>
#include <vector>

#define JSON_EXPT_RND 1 // Generic exception
#define JSON_EXPT_ESC 2 // Bad escape sequence

#define JSON_TYPE_OBJ 1
#define JSON_TYPE_ARR 2
#define JSON_TYPE_STR 3

/**
 * json.hh
 *
 * A single file JSON parser.
 **/
class JSON{
  private:
    int rawLen;
    int type;
    std::string k;
    std::string v;
    std::vector<JSON*> childs;

    /**
     * JSON()
     *
     * Parse the JSON string and generate the relevant children objects.
     *
     * @param json The valid JSON input String.
     * @param offset The offset to start parsing from in the String.
     **/
    private: JSON(std::string json, int offset){
      /* Setup internal variables */
      rawLen = 0;
      type = JSON_TYPE_STR;
      /* Parse for object type */
      bool string = false;
      bool escape = false;
      bool keyFill = true;
      for(int x = offset; x < json.length(); x++){
        char c = json[x];
        /* Parse JSON structure */
        if(!string){
          switch(c){
            case '{' :
            case '[' :
              /* Set type if not set */
              if(type == JSON_TYPE_STR){
                /* Check special case root object */
                if(offset > 0){
                  type = c == '{' ? JSON_TYPE_OBJ : JSON_TYPE_ARR;
                }else{
                  offset = 1;
                }
              /* Handle this child object */
              }else{
                JSON* child = new JSON(json, x);
                childs.push_back(child);
                x += child->getRawLen();
              }
              break;
            case '}' :
            case ']' :
              /* Parent didn't bare children, don't double recursion escape */
              if(type != JSON_TYPE_STR && childs.size() <= 0){
                ++x;
              }
            case ',' :
              /* End of this object */
              rawLen = x - offset;
              return;
            case '"' :
              if(type == JSON_TYPE_STR){
                string = !string;
              }else{
                JSON* child = new JSON(json, x);
                childs.push_back(child);
                x += child->getRawLen();
              }
              break;
            case ':' :
              keyFill = false;
              break;
          }
        /* Parse JSON string */
        }else{
          /* If not an escape character */
          if(c != '\\' || escape){
            /* Flip string state if not escaping */
            if(c == '"' && !escape){
              string = !string;
            }
            /* Pre-escape if needed */
            if(escape){
              switch(c){
                case 'b' :
                  c = '\b';
                  break;
                case 'f' :
                  c = '\f';
                  break;
                case 'n' :
                  c = '\n';
                  break;
                case 'r' :
                  c = '\r';
                  break;
                case 't' :
                  c = '\t';
                  break;
                case '"' :
                  c = '"';
                  break;
                case '\\' :
                  c = '\\';
                  break;
                default :
                  throw JSON_EXPT_ESC;
              }
              escape = false;
            }
            /* Are we still in the string? */
            if(string){
              /* Where are we putting this? */
              if(keyFill){
                k += c;
              }else{
                v += c;
              }
            }
          /* Handle escape case */
          }else{
            escape = true;
          }
        }
      }
    }

    /**
     * JSON()
     *
     * Parse the JSON string and generate the relevant children objects.
     *
     * @param json The valid JSON input String.
     **/
    public: JSON(std::string json) : JSON('{' + json + '}', 0){
      /* Pass through constructor */
    }

    /**
     * build()
     *
     * A factory builder to parse the JSON string from a file and generate the
     * relevant child objects.
     *
     * @param filename A valid file containing the JSON data.
     **/
    public: static JSON* build(std::string filename){
      /* Load the file */
      std::ifstream file(filename);
      std::string json = "";
      std::string line;
      while(std::getline(file, line)){
        json += line;
      }
      /* Close the file to reserve resources */
      file.close();
      /* Parse from root */
      return new JSON(json);
    }

    /**
     * getRawLen()
     *
     * Get the raw parser length of what was parsed. This is intended to be used
     * by the constructor during parsing.
     *
     * @return The raw parser length in number of bytes.
     **/
    public: int getRawLen(){
      return rawLen;
    }

    /**
     * isObject()
     *
     * Check whether this is an object.
     *
     * @param True if an object, otherwise false.
     **/
    public: bool isObject(){
      return type == JSON_TYPE_OBJ;
    }

    /**
     * isArray()
     *
     * Check whether this is an array.
     *
     * @param True if an array, otherwise false.
     **/
    public: bool isArray(){
      return type == JSON_TYPE_ARR;
    }

    /**
     * isString()
     *
     * Check whether this is an string.
     *
     * @param True if an string, otherwise false.
     **/
    public: bool isString(){
      return type == JSON_TYPE_STR;
    }

    /**
     * key()
     *
     * Get the key for this JSON object. NOTE: Array elements may not have a key.
     *
     * @param def The default value to return if the key is not set.
     * @return The key, otherwise the default key.
     **/
    public: std::string key(std::string def){
      return !k.empty() ? k : def;
    }

    /**
     * value()
     *
     * Get the value for this JSON object. NOTE: Only strings will have keys.
     *
     * @param def The default value to return if the value is not set.
     * @return The value, otherwise the default value.
     **/
    public: std::string value(std::string def){
      return !v.empty() ? v : (!k.empty() ? k : def);
    }

    /**
     * length()
     *
     * The number of children elements this JSON object has.
     *
     * @return The number of child elements, otherwise zero.
     **/
    public: int length(){
      if(!childs.empty()){
        return childs.size();
      }else{
        return 0;
      }
    }

    /**
     * get()
     *
     * Get a child element of this JSON object. NOTE: Only objects and arrays can
     * have child elements.
     *
     * @param x The index of the element to retrieve.
     * @return The JSON object at the given location, otherwise this object.
     **/
    public: JSON* get(int x){
      if(!childs.empty() && x >= 0 && x < childs.size()){
        return childs[x];
      }else{
        return this;
      }
    }

    /**
     * get()
     *
     * Get a child element of this JSON object by key. NOTE: Only objects and
     * arrays can have child elements.
     *
     * @param k The key to be used to search for the element.
     * @return The JSON object at the given location, otherwise this object.
     **/
    public: JSON* get(std::string k){
      if(!childs.empty() && !k.empty()){
        for(int x = 0; x < childs.size(); x++){
          JSON* c = childs[x];
          if(c != NULL && !c->key("").empty() && c->key("").compare(k) == 0){
            return c;
          }
        }
      }
      return this;
    }

    /**
     * exists()
     *
     * Check whether a child element exists.
     *
     * @param k The key to be used to search for the element.
     * @return True if the child exists, otherwise false.
     **/
    public: bool exists(std::string k){
      if(!childs.empty() && !k.empty()){
        for(int x = 0; x < childs.size(); x++){
          JSON* c = childs[x];
          if(!c->key("").empty() && c->key("").compare(k) == 0){
            return true;
          }
        }
      }
      return false;
    }

    /**
     * toString()
     *
     * Convert this object and all child objects to a printable String.
     *
     * @return A printable String representing this object and it's child
     * elements.
     **/
    public: std::string toString(){
      std::string a;
      std::string o;
      switch(type){
        case JSON_TYPE_OBJ :
          o = !k.empty() ? "\"" + k + "\":{" : "{";
          if(!childs.empty()){
            for(int x = 0; x < childs.size(); x++){
              JSON* c = childs[x];
              o += c->toString();
              o += x < childs.size() - 1 ? "," : "";
            }
          }
          return o + '}';
        case JSON_TYPE_ARR :
          a = !k.empty() ? "\"" + k + "\":[" : "[";
          if(!childs.empty()){
            for(int x = 0; x < childs.size(); x++){
              JSON* c = childs[x];
              a += c->toString();
              a += x < childs.size() - 1 ? "," : "";
            }
          }
          return a + ']';
        case JSON_TYPE_STR :
          if(!k.empty() && !v.empty()){
            return '\"' + k + "\":\"" + v + '\"';
          }else if(!k.empty()){
            return '\"' + k + '\"';
          }else if(!v.empty()){
            return '\"' + v + '\"';
          }else{
            return "";
          }
        default :
          return "";
      }
    }

    /**
     * assurt()
     *
     * A very simple assertion method for testing that the JSON parser isn't
     * regressing.
     *
     * @param r The result to test.
     * @return Pass through for the result value.
     **/
    #define assurt(r) ({ \
      printf("[%s] %s::%i\n", r ? "PASS" : "FAIL", __FUNCTION__, __LINE__); \
      r; \
    })

    /**
     * test()
     *
     * Test that the parser works as expected. The result of each test and a
     * summary is printed to the standard out, as well as a boolean indicating
     * test success.
     *
     * @return The result of performing the tests, true if success, otherwise
     * false.
     **/
    public: static bool test(){
      /* Setup variables */
      std::vector<std::string> test;
      test.push_back("{}");
      test.push_back("{\"test\"}");
      test.push_back("{\"test\":\"123\"}");
      test.push_back("{\"test\":{}}");
      test.push_back("{\"test\":[]}");
      test.push_back("{\"test\":{\"arg-a\",\"arg-b\"}}");
      test.push_back("{\"test\":[\"arg-a\",\"arg-b\"]}");
      test.push_back("{\"test\":{\"arg-a\":\"123\",\"arg-b\":\"456\"}}");
      test.push_back("{\"test\":[\"arg-a\":\"123\",\"arg-b\":\"456\"]}");
      test.push_back("{\"test\":{{\"arg-a\":\"123\"},{\"arg-b\":\"456\"}}}");
      test.push_back("{\"test\":[{\"arg-a\":\"123\"},{\"arg-b\":\"456\"}]}");
      test.push_back("{\"1\":[\"a\"],\"2\":[\"b\"]}");
      test.push_back("{\"a\":\"b\",\"c\":{\"d\":\"e\",\"f\":{},\"g\":\"h\"}}");
      test.push_back("{\"a\":\"b\",\"c\":{\"d\":\"e\",\"f\":[],\"g\":\"h\"}}");
      bool r = true;
      /* Run parser tests */
      for(int x = 0; x < test.size(); x++){
        bool a = assurt((new JSON(test[x]))->toString().compare(test[x]) == 0);
        if(!a){
          printf("  in:  '%s'\n", test[x].c_str());
          printf("  got: '%s' !=\n", (new JSON(test[x]))->toString().c_str());
          printf("  exp: '%s'\n", test[x].c_str());
        }
        r &= a;
      }
      /* Run getter tests */
      r &= assurt((new JSON("{\"test\":[{\"arg-a\":\"123\"},{\"arg-b\":\"456\"}]}"))
        ->get(0)->get(0)->toString().compare("{\"arg-a\":\"123\"}") == 0);
      r &= assurt((new JSON("{\"test\":[\"arg-a\":\"123\",\"arg-b\":\"456\"]}"))
        ->get(0)->get("arg-b")->toString().compare("\"arg-b\":\"456\"") == 0);
      /* Print result */
      printf("\n");
      printf("  Tests %s\n", r ? "PASSED" : "FAILED");
      return r;
    }
};
