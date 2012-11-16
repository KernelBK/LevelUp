// XMLTreeRenderer.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

//sibling��չ�ķ����Ϊsibling direction, sibdir
//parent��չ�ķ����Ϊparent direction, pardir
#include "stdafx.h"

#include "LayoutTree.h"
#include "TextLayoutTreeNode.h"

#define TIXML_USE_STL
#include <tinyxml.h>
#include "inc_gdiplus.h"

#include <eflib/include/eflib.h>
#include <boost/python.hpp>

#include <string>
#include <iostream>

using namespace std;
using namespace Gdiplus;

class GDIPlusInitializer
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
public:
	GDIPlusInitializer(){
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}
	~GDIPlusInitializer(){
		GdiplusShutdown(gdiplusToken);
	}
};

void ParseElemToLayoutNodeRecursive(
									TiXmlElement* elem,
									TextLayoutTreeNode* parent_node,
									LayoutTree* owner,
									Font* font,
									Graphics* g,
									int width,
									StringFormat* strfmt
									)
{
	std::string elem_str;
	elem_str += elem->Attribute("name");
	elem_str += "\n(";
	elem_str += elem->Attribute("global_weight");
	elem_str += ")";

	TextLayoutTreeNode* cur_node = owner->create_node<TextLayoutTreeNode>(parent_node);
	cur_node->set_renderer_params(font, width, g, elem_str, strfmt);

	for(TiXmlElement* child = elem->FirstChildElement("feature"); child != NULL; child = child->NextSiblingElement("feature"))
	{
		ParseElemToLayoutNodeRecursive(child, cur_node, owner, font, g, width, strfmt);
	}
	
}

void ParseDomToLayoutTree(const std::string& xmlFile,
						  LayoutTree* tree,
						  Font* font,
						  Graphics* g,
						  int width,
						  StringFormat* strfmt)
{
	TiXmlDocument doc;
	doc.LoadFile(xmlFile);
	ParseElemToLayoutNodeRecursive(doc.FirstChildElement("feature"), NULL, tree, font, g, width, strfmt);
}

void RenderXMLTree(const std::string& xmlFile, const std::string& outputFile){
	//���г�ʼ��
	static GDIPlusInitializer __init_gdiplus;

	//�������Ĺ���Ҫ��
	LayoutTree tree;
	tree.padding(10.0f);
	tree.pardir_spacer(20.0f);
	tree.sibdir_spacer(20.0f);

	//Ԥ�������Ԫ��
	Bitmap* bmp = new Bitmap(1, 1, PixelFormat32bppARGB);
	Graphics* g = Graphics::FromImage(bmp);
	g->SetSmoothingMode(SmoothingModeAntiAlias);

	Font font(L"����", 24);
	StringFormat strfmt;
	strfmt.SetAlignment(StringAlignmentCenter);
	strfmt.SetLineAlignment(StringAlignmentCenter);

	//������ڵ㣬�����㲼��
	ParseDomToLayoutTree(xmlFile, &tree, &font, g, 10, &strfmt);
	tree.root()->calculate_size_recursive();
	tree.root()->update_layout();

	//�������λͼ
	delete bmp;
	bmp = new Bitmap(tree.width(), tree.height(), PixelFormat32bppARGB);
	delete g;
	g = Graphics::FromImage(bmp);
	g->SetSmoothingMode(SmoothingModeHighQuality);

	//���¹������ڵ㲢�����λͼ�Ͻ��л���
	tree.clear();
	ParseDomToLayoutTree(xmlFile, &tree, &font, g, 10, &strfmt);
	tree.root()->calculate_size_recursive();
	tree.root()->update_layout();
	tree.root()->render();

	//����λͼ
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	bmp->Save(to_wide_string(outputFile).c_str(), &pngClsid, NULL);

	delete bmp;
	delete g;
}

BOOST_PYTHON_MODULE(XMLTreeRenderer)
{
	using namespace boost::python;
	def("RenderXMLTree", RenderXMLTree);
}