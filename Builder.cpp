#include <iostream>
#include <vector>


using namespace std;

class TextConverter{
    /*abstract class*/
    public:
    virtual void ConvertCharacter(char temp) = 0;
    virtual void ConvertFontChange(string temp) = 0;
    virtual void ConvertParagraph (string temp) = 0;
};

class ASCIIConverter : public TextConverter{
    private:
    string result = "";

    public:
    void ConvertCharacter(char temp) override{
        result += temp;
    }
    void ConvertFontChange(string temp) {}
    void ConvertParagraph (string temp) {}
    string getASCIIText (){
        return result;
    }
};

class TeXConverter :public TextConverter{
    private:
    string result = "";

    public:
    void ConvertCharacter(char temp){
        result += temp;
    }
    void ConvertFontChange(string temp){
        result += temp;
        result += " {font_TeXCon.} ";
    }
    void ConvertParagraph (string temp){
        result += temp;
        if(temp == "\\par")
        result += " {end of paragraph settings {TeXConverter}} \n";
    }
    string getTeXText (){
        return result;
    }
};

class TextWidgtConverter :public TextConverter{
    private:
    string result = "";

    public:
    void ConvertCharacter(char temp){
        result += temp;
    }
    void ConvertFontChange(string temp){
        result += temp;
        result += " {font_TeXWid.} ";
    }
    void ConvertParagraph (string temp){
        result += temp;
        if(temp == "\\par")
        result += " {end of paragraph settings {TeXWidegt}} \n";
    }
    string getTeXWidgt (){
        return result;
    }
};

class RTFReader{
    private:
    vector <pair<string,string>> tokens;
    TextConverter* builder = nullptr;

    public:
    /*parsing according to the instance of the builder will be directed to it*/
    void ParseRTF(){
        for (int i = 0; i < tokens.size(); i++){
            if(tokens[i].first == "WORD"){
                for (int j = 0; j < tokens[i].second.size(); j++){
                    builder->ConvertCharacter(tokens[i].second[j]);
                }
            }
            else if(tokens[i].first == "PARA"){
                builder->ConvertParagraph(tokens[i].second);
            }
            else if(tokens[i].first == "FONT"){
                builder->ConvertFontChange(tokens[i].second);
            }
        }
    }
/*simulate the scanning into tokens but really simple*/
    void ScanRTF(string file){
        int i = 0;
        while(i<file.size()){
            if(file[i]=='\\'){
                int j = i+1;
                while (j<file.size() && file[j]!='\\' && file[j]!=' '){
                    j++;
                }
                string temp;
                temp = file.substr(i,j-i);
                if(temp == "\\b" || temp == "\\i" || temp == "\\ul" || temp == "\\b0" ) tokens.push_back({"FONT",temp});
                else tokens.push_back({"PARA",temp});
                i = j;
            }
            else{
                int j = i;
                while (j<file.size() && file[j]!='\\'){
                    j++;
                }
                string temp;
                temp = file.substr(i+1,j-i-1);
                tokens.push_back({"WORD",temp});
                i = j;
            }
            
        }
    }
    /*setting builder for the parsing function*/
    void setBuilder (TextConverter* temp){
        builder = temp;
    }

};

int main(){
    /* Sample file*/
    string s =  R"(\pard\qc\fs40\b\cf2 Welcome to the RTF Example File! \b0\cf0\par This is a sample Rich Text Format (RTF) document. \b Bold text \i italic text \ul underlined text)";
    
    /* Scanning file and saving tokens in class RTF reader*/
    RTFReader reader;
    reader.ScanRTF(s);

    /* iniatlizing builder inside RTF reader to be ASCII and parsing */
    ASCIIConverter ASCII_builder;
    reader.setBuilder(&ASCII_builder);
    reader.ParseRTF();
    cout<<"\n\n"<<"ASCII output: \n"<<ASCII_builder.getASCIIText()<<endl;

    /* iniatlizing builder inside RTF reader to be TeXConverter and parsing */ 
    TeXConverter TeXConverter_builder;
    reader.setBuilder(&TeXConverter_builder);
    reader.ParseRTF();
    cout<<"\n\n"<<"TeXConverter output: \n"<<TeXConverter_builder.getTeXText()<<endl;

    /* iniatlizing builder inside RTF reader to be TextWidgtConverter and parsing */ 
    TextWidgtConverter TextWidgtConverter_builder;
    reader.setBuilder(&TextWidgtConverter_builder);
    reader.ParseRTF();
    cout<<"\n\n"<<"TextWidgtConverter output: \n"<<TextWidgtConverter_builder.getTeXWidgt()<<endl;     
}
