#include <string>
#include <map>
#include <vector>
#include <stack>
#include<algorithm>

namespace tiny{
    class Value;
    class Json;

    class Value{
        public:
            char type_ = '_'; // '!': error, '_': null, 's': str, 'n': num_, 'j': json;
            int errCode_ = 0;
        private:
            std::string str_;
            int num_;
            Json *js_;
        public:
            Value(std::string str) : type_('s'), num_(-1), js_(nullptr), str_(str){}
            Value(int num) : type_('i'), num_(num), js_(nullptr), str_(""){}
            Value(Json *js) { setValue(js); }
            Value(Json &js) { setValue(&js); }
            Value(int errCode, std::string errReason) : type_('!'), errCode_(errCode), js_(nullptr), str_(errReason) {}
            ~Value(){delete js_; }
        public:
            Value add(Value &aV);
            void copy(Value &aV);
            void addToSelf(Value &aV);
            void clear(){ type_ = '_'; num_ = -1; str_ = ""; delete(js_); js_ = nullptr; }
            void setValue(std::string str)  { clear(); type_ = 's'; str_ = str; }
            void setValue(int num)          { clear(); type_ = 'i'; num_ = num; }
            void setValue(Json *js);
            void setValue(int errCode, std::string errReason){ clear(); type_ = '!'; num_ = errCode; str_ = errReason; }
            char getType()          { return type_; } 
            std::string getStr()    { return str_; }
            int getNum()            { return num_; }
            Json* getJson()         { return js_; }
            std::string getErrReason(){ return str_; }
            int getErrCode(){ return errCode_; }
        public:
            void operator = (Value &aValue){ copy(aValue); }
            Value operator + (Value &aValue){ return add(aValue); }
            void operator += (Value &aValue){ return addToSelf(aValue); }
    };






    class Json{
        public:
            char type_ = '_'; // '_': null, 'k': key-value json, 'v': list(value json)
            std::string errReason_;
            int errCode_ = 0; 
        private: 
            std::vector<Value> values_;
            std::map<std::string, Value> key_values_;
        public: // 初始化
            Json(){}
            Json(std::string str){parse(str);}
            Json(int errCode, std::string errReason) : type_('!'), errCode_(errCode), errReason_(errReason){}
            Json(std::vector<Value> values): type_('v'), values_(values){}
            Json(std::map<std::string, Value> key_values): type_('k'), key_values_(key_values){}
            Json(Json *js) : type_(js->getType()), values_(js->getValues()), key_values_(js->getKeyValues()) {copy(js);}
            ~Json(){ delete(&values_); delete(&key_values_); } 
        public: //基础函数
            void copy(Json *js){
                clear();
                type_ = js->getType();
                if(type_ == 'k') key_values_ = js->getKeyValues();
                else values_ = js->getValues(); 
            }
            void copy(Json &js){ copy(&js); }
            void clear(){ 
                type_ = '_'; 
                values_.clear(); 
                key_values_.clear(); 
                errCode_ = 0;
                errReason_.clear();
            }
            void addToSelf(Json *js){
                     if(type_ == 'k' && js -> getType() == 'k') return (key_values_.insert(js -> getKeyValues().begin(), js -> getKeyValues().end()));
                else if(type_ == 'v' && js -> getType() == 'v') values_.insert(values_.end(), js -> getValues().begin(), js -> getValues().end());
                else if(type_ == 'v' && js -> getType() == 'k') values_.push_back(Value(js));
                else if(type_ == 'k' && js -> getType() == 'v') {
                    clear();
                    type_ = 'v';
                    values_ = js->getValues();
                    values_.push_back(Value(js));
                }
                else return;
            }
            void addToSelf(Json &js){ addToSelf(&js); }
            Json add(Json *js){
                     if(type_ == 'k' && js -> getType() == 'k') return mapAddMap(key_values_, js->getKeyValues());
                else if(type_ == 'v' && js -> getType() == 'v') return vecAddVec(values_, js->getValues());
                else if(type_ == 'v' && js -> getType() == 'k') return vecAddMapJson(values_, js);
                else if(type_ == 'k' && js -> getType() == 'v') return vecAddMapJson(js->getValues(), &Json(key_values_));
                else return *js;
            }
            Json add(Json &js){ return add(&js); }
            char getType(){ return type_; }
            int  getErrCode() { return errCode_; }
            std::string getErrReason() { return errReason_; }
            std::vector<Value> getValues(){ return values_; }
            std::map<std::string, Value> getKeyValues(){ return key_values_; }
            void setJson(std::vector<Value> &values){ clear(); type_='v'; values_=values; }
            void setJson(std::map<std::string, Value> &key_values){ clear(); type_='k'; key_values_=key_values; }
            
        public:  //解析函数
            void parse(std::string str);
            std::string trim(std::string &s, char lc, char rc){ //剥掉最外一层皮
                std::string ss;
                if (s.find(lc) != std::string::npos && s.find(rc) != std::string::npos) {
                    size_t b = s.find_first_of(lc);
                    size_t e = s.find_last_of(rc);
                    ss = s.substr(b + 1, e - b - 1);
                }
                return ss;               
            }

        public: //支持函数
            std::map<std::string, Value> mapAddMap(std::map<std::string, Value> m1, std::map<std::string, Value> &m2){
                m1.insert(m2.begin(), m2.end());
                return m1;
            }
            std::vector<Value> vecAddVec(std::vector<Value> v1, std::vector<Value> &v2){
                v1.insert(v1.end(), v2.begin(), v2.end());
                return v1;
            }
            std::vector<Value> vecAddMapJson(std::vector<Value> v, Json *js){
                v.push_back(Value(js));
                return v;
            }
            std::string jsonFormat(std::string &str){ // 去除" "(双引号)之外的空格与换行
                std::string ss;
                int flag = 1;
                for(auto i : str){
                    if(i == '"') flag *= -1;
                    if(flag == -1) ss += i;
                    else if (i != ' ' && i != '\n' && i != '\t') ss += i; 
                } 
                return ss;
            }
            std::string Trims(std::string s, char lc, char rc){ // 取lc和rc两个字符之间的字符串(从两端开始找)
                std::string ss = s;
                if (s.find(lc) != std::string::npos && s.find(rc) != std::string::npos) {
                    size_t b = s.find_first_of(lc);
                    size_t e = s.find_last_of(rc);
                    ss = s.substr(b + 1, e - b - 1);
                }
                return ss;
            }
            std::string FetchStrStr(std::string inputstr, int &offset){
                int tokencount = 0;
                std::string objstr;
                bool hasFound = false;
                size_t i = 0;
                for (; i < inputstr.size(); i++) {
                    char c = inputstr[i];
                    if (!hasFound && c == '"') hasFound = true;
                    else if(hasFound && c == '"') break;
                    else if (hasFound) objstr.push_back(c);
                }
                offset = i + 1;
                return objstr;
            }

            std::string FetchNumStr( std::string inputstr, int& offset ){
                    std::string objstr;
                    size_t i = 0;
                    bool hasFound = false;
                    for(; i < inputstr.size(); ++i){
                        if((inputstr[i] > '0' && inputstr[i] < '9') || inputstr[i] == '+' || inputstr[i] == '-'){
                            objstr.push_back(inputstr[i]);
                            hasFound = true;
                        }else{
                            if(hasFound)
                                break;
                        }
                    }
                    offset = i;
                    return objstr;
            }

            std::string FetchArrayStr(std::string inputstr, int& offset, bool hasKey = 0){
                char lc = hasKey ? '{' : '[';
                char rc = hasKey ? '}' : ']';
                size_t i = 0;
                std::string objstr;
                int tokencount = 0;
                bool hasFound = false;
                for (; i < inputstr.size(); i++) {
                    char c = inputstr[i];
                    if (c == lc) {
                        hasFound = true;
                        tokencount++;
                    }
                    else if (c == rc) tokencount--;
                    if(tokencount == 0 && hasFound == true) break;
                    if (hasFound == true) objstr.push_back(c);
                }
                offset = i + 1;
                return objstr.substr(1);
            }
            std::string getFirstStr(std::string &str, int &offset){
                std::string temp = "";
                if(str[0] == '"')       temp += ("s" + FetchStrStr(str, offset));
                else if (str[0] == '[') temp += ("v" + FetchArrayStr(str, offset, 0));
                else if (str[0] == '{') temp += ("k" + FetchArrayStr(str, offset, 1));
                else                    temp += ("n" + FetchNumStr(str, offset));
                if(!str.empty())        str = str.substr(offset + 1);
                return temp;
            }

        public: // 插入一个 value 或者一个 key-value 对
            void insert(std::string key, std::string str){ key_values_.insert(std::pair<std::string, Value>(key, Value(str))); }
            void insert(std::string key, int num){ key_values_.insert(std::pair<std::string, Value>(key, Value(num))); }
            void insert(std::string key, Json *js){ key_values_.insert(std::pair<std::string, Value>(key, Value(js))); }
            void insert(std::string key, Value &aValue){ key_values_.insert(std::pair<std::string, Value>(key, aValue)); }
            void insert(std::string str){ values_.push_back(Value(str)); }
            void insert(int num){ values_.push_back(Value(num)); }
            void insert(Json *js){ values_.push_back(Value(js)); }
            void insert(Value &aValue){ values_.push_back(aValue); }
        
        public: //存在性判别
            bool hasKey(std::string key){return (key_values_.count(key) > 0) ? true : false; }
            bool hasId(int id){ return ((id <= values_.size())) ? false : true; }
        
        public: // 由 key/id 获取对应的 Value (int, string, Json)
            Value getValueByKey(std::string key){
                if(type_ == 'v') return Value(22, "错误的引用");
                std::map<std::string,Value>::iterator it = key_values_.find(key);
                return (it == key_values_.end()) ? Value(11, "键不存在") : it->second;
            }
            Value getValueById(int id){
                if(type_ == 'k') return Value(21, "错误的引用"); 
                return(! hasId(id)) ? Value(12, "索引超限") : values_[id]; 
            }      
        public: // 删除一个 value 或者 key-value 对
            bool deleteByKey(std::string key){
                if (!hasKey(key)) return false;
                std::map<std::string,Value>::iterator it = key_values_.find(key);
                key_values_.erase (it);
                return true;
            }
            bool deleteById(int id){
                if (!hasId(id)) return false;
                values_.erase(values_.begin() + id);
                return true;
            }
            template <class T> bool deleteByValue(T aT){
                Values aValue(aT);
                if (type_ == 'v'){
                    std::vector<Value>::iterator it;
                    it = values_.find(aValues);
                    if (it == key_values_.end())
                        return false;
                    values_.erase(it);
                    return true;
                }
                else if (type_ == 'k'){
                    std::map<std::string,Value>::iterator it = key_values_.end();
                    it = std::find_if(key_values_.begin(), key_values_.end(), map_value_finder(aValue));
                    if (it == key_values_.end()) return false;
                    key_values_.erase(it);
                    return trun;
                    
                }
                else{
                    return false;
                }
            }
        
        public: // 修改键值对
            template <class T>
            bool edit(std::string key, T aT){
                std::map<std::string,Value>::iterator it = key_values_.find(key);
                if (it == key_values_.end()) return false;
                it->second = Value(aT);
                return true;
            }
            template <class T>
            bool edit(int id, T aT){
                if (!hasId) return false;
                values_[id] = Value(aT);
                return true;
            }
        
        public: // json -> str
            std::string toString(){
                std::string str = "";
                if (type_ == 'v'){
                   str += "[";
                   for (auto it : values_){
                       if       (it.getType() == 's')    str += ( "\"" + it.getStr() + "\"" );
                       else if  (it.getType() == 'i')    str += std::to_string(it.getNum());
                       else if  (it.getType() == 'j')    str += it.getJson()->toString();
                       str += ", ";
                   }
                   str = (str.substr(0, str.size() - 2) + "]");
                //    str += (*( str.end() - 1) == '}' || *( str.end() - 1) == '}') ? "]" : " ]";
                }
                else if (type_ == 'k'){
                    str += "{";
                    for (auto it : key_values_){
                        str += ( "\"" + it.first + "\": ");
                        if      (it.second.getType() == 's')    str += ( "\"" + it.second.getStr() + "\"" );
                        else if (it.second.getType() == 'i')    str += std::to_string( it.second.getNum() );
                        else if (it.second.getType() == 'j')    str += it.second.getJson() -> toString();
                        str += ", ";
                   }
                   str = ( str.substr(0, str.size() - 2) + "}");
                //    str += (*( str.end() - 1) == '}' || *( str.end() - 1) == '}') ? "}" : " }";
                }
                return str;
            }

        public:  // 重载  
            void operator =  (Json *js){ copy(js); }
            void operator += (Json *js){ addToSelf(js); }
            void operator += (Json js){ addToSelf(js); }
            void operator += (std::string str){ values_.push_back(Value(str)); }
            void operator += (int num){ values_.push_back(Value(num)); }
            void operator += (Value &aValue){ values_.push_back(aValue); }
            Json operator +  (Json *js){ return add(js); }
            Json operator +  (Json &js){ return add(js); }
            Value &operator [] (int id){
                if(type_ == 'k') return Value(21, "错误的引用");
                return(! hasId(id)) ? Value(12, "索引超限") : values_[id]; 
                }
            Value &operator [] (std::string key){
                if (type_ == 'v') return Value(22, "错误的引用");
                std::map<std::string,Value>::iterator it = key_values_.find(key);
                return (it == key_values_.end()) ? Value(11, "值不存在") : it->second;
            }
            
    };

    Value Value::add(Value &aV){
            if (type_ == 's' && aV.getType() == 's') return Value(str_ + aV.getStr()); 
        else if (type_ == 'i' && aV.getType() == 'i') return Value(num_ + aV.getNum());
        else if (type_ == 'j' && aV.getType() == 'j') return aV;//TODO
        else if (type_ == 's' && aV.getType() == 'i') return Value(str_ + std::to_string(aV.getNum()));
        else if (type_ == 's' && aV.getType() == 'i') return Value(std::to_string(num_) + aV.getStr());
        else if (type_ == '_' ) return aV;
        else if (aV.getType() == '_') return *this;
        else return Value(30, "类型不匹配");
    }

    void Value::addToSelf(Value &aV){
            if (type_ == 's' && aV.getType() == 's')    str_ += aV.getStr(); 
        else if (type_ == 'i' && aV.getType() == 'i')    num_ += aV.getNum();
        else if (type_ == 'j' && aV.getType() == 'j')    return ;//TODO
        else if (type_ == 's' && aV.getType() == 'i')    str_ + std::to_string(aV.getNum());
        else if (type_ == 's' && aV.getType() == 'i')  { clear(); setValue(std::to_string(num_) + aV.getStr()); }
        else if (type_ == '_' && aV.getType() == '_')    return;
        else setValue(30, "类型不匹配");
    }

    void Value::copy(Value &aV){
        type_ = aV.getType();
        num_ = aV.getNum();
        str_ = aV.getStr();
        js_ = new Json(aV.getJson());

    }

    void Value::setValue(Json *js){
        type_ = 'j';
        num_ = -1;
        str_ = "";
        js_ = new Json(js);
    }

    void Json::parse(std::string str){
        clear();
        str = jsonFormat(str);
        if(str[0] == '{'){
            type_ = 'k';
            str = Trims(str, '{', '}');
            int offset;
            str += "  ";
            std::string temp, temp2;
            while(str[0] == '"'){
                temp = getFirstStr(str, offset).substr(1);
                temp2 = getFirstStr(str, offset);
                if (temp2[0] == 'n') key_values_.insert({temp, Value(std::stoi(temp2.substr(1)))});
                else if (temp2[0] == 's') key_values_.insert({temp, Value(temp2.substr(1))});
                else if (temp2[0] == 'k') {
                    Json j = new Json();
                    j.parse("{" + temp2.substr(1) + "}");
                    key_values_.insert({temp, Value(j)});
                }
                else if (temp2[0] == 'v') {
                    Json j = new Json();
                    j.parse("[" + temp2.substr(1) + "]");
                    key_values_.insert({temp, Value(j)});
                }
            }

        }
        else if(str[0] == '['){
            type_ = 'v';
            str = Trims(str, '[', ']');
            int offset;
            str += "  ";
            std::string temp;
            while(str[0] == '"' || str[0] == '{' || str[0] == '[' || (str[0] > '0' && str[0] < '9')){
                temp = getFirstStr(str, offset);
                if (temp[0] == 'n') values_.push_back( Value(std::stoi(temp.substr(1))) );
                else if (temp[0] == 's') values_.push_back( Value(temp.substr(1)));
                else if (temp[0] == 'k') {
                    Json j = new Json();
                    j.parse("{" + temp.substr(1) + "}");
                    values_.push_back( Value(j) );
                }
                else if (temp[0] == 'v') {
                    Json j = new Json();
                    j.parse("[" + temp.substr(1) + "]");
                    values_.push_back( Value(j) );         
                }       
            }
        }    
    }


    std::string allQuotToDouble(std::string str){ // 替换: ' -> " 
        replace(str.begin(), str.end(), '\'', '"');
        return str;
    }



}
