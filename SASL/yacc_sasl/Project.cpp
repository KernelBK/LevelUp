#include "Project.h"
#include "Makefile.h"
#include "cstrext.h"

#undef yyFlexLexerOnce

#include "YYLexer.h"
#include "YYParser.h"

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

using namespace std;
using namespace boost::filesystem;

/***
error_reporter
***/
string error_reporter_tags::compiler_config_error(){
	return "Compiler configuration Error: ";
}

string error_reporter_tags::syntax_error(){
	return "Syntax Error: ";
}

/**
unit
**/
unit::unit()
{
	this->error_reporter_ = h_error_reporter(proj().get_error_reporter()->clone());
}

context* unit::get_current_context(){
	return context_.get();
}

symbol_table* unit::get_global_scope()
{
	return symtbl_.get();
}

bool unit::parse(const string& filePath){
		fstream fs;
		fs.open(filePath.c_str(), ios::in);
		if (!fs.is_open()){
			error_reporter_->ReportCompilerError(format("%s\"%s\"%s", "�����ļ�", filePath.c_str(), "�޷���!"));
		}

		Lexer* pLexer = new Lexer(&fs, NULL, this);
		yyparse(pLexer);

		fs.close();
		return true;
}

/****
project
****/
void project::initialize(h_preprocessor pp, h_error_reporter error_reporter){
	proj().pp_ = pp;
	proj().err_reporter_ = error_reporter;
}

void project::parse(const string& configName){
	configuration* pConf = get_config(configName);
	if(!pConf){ return; }
	if(!parse_units(pConf)){	return; }
}

project& project::instance(){
	return prj_;
}

error_reporter* project::get_error_reporter(){
	return err_reporter_.get();
}

project::project()
	:mf_(makefile::from_directory(current_path().string()))
{
}

project::~project(){
}

project project::prj_;

/**
project Inner Implementation
**/
configuration* project::get_config(const std::string& configName)
{
	if (!mf_){
		err_reporter_->report_compile_internal_error("Makefile��ȡʧ��!");
		return NULL;
	}

	if (mf_->Configurations().empty()){
		err_reporter_->report_compile_internal_error("Makefileû���ҵ���Ч�ı������á�");
		return NULL;
	}

	if(configName.empty())
	{
		return &(mf_->Configurations()[0]);
	} else {
		BOOST_FOREACH(configuration& conf, mf_->Configurations())
		{
			if(conf.ID() == configName){
				return &conf;
			}
		}
	}

	err_reporter_->ReportCompilerError(
		format("%s\"%s\"%s", "Makefileû����Ϊ", configName.c_str(), "�ı������á�")
		);
	return NULL;
}

//�ֵ�Ԫ(Դ�ļ�)���з���
bool project::parse_units(configuration* pconf)
{
	if( pconf->get_inputs().size() == 0){
		err_reporter_->ReportCompilerError("ȱ�������ļ�!");
		return false;
	}

	BOOST_FOREACH (string filename, pconf->get_inputs())
	{
		string fileFullPath = pconf->get_work_environment().find_path(filename);
		fstream fs;
		fs.open(fileFullPath.c_str(), ios::in);
		if (!fs.is_open()){
			err_reporter_->ReportCompilerError(format("%s\"%s\"%s", "�����ļ�", filename.c_str(), "�޷���!"));
		}

		//����Դ�ļ��Ժ󴴽�һ��Unit
		units_.push_back(HUnit(new unit()));

		//ִ�з�������
		units_.back()->Parse(fileFullPath);

		fs.close();
	}

	return true;
}
