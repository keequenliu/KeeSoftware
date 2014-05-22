#include "CoreViewer.h"

#include <osgDB/ReadFile>
#include <osgDB/Registry>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/Matrix>
#include <osg/CullFace>
#include <osg/ShadeModel>

#include <osg/PolygonMode>
#include <osg/Texture2D>

#include "RenderState.h"

CoreViewer::CoreViewer()
{
    init();
}

CoreViewer::~CoreViewer()
{

}

void CoreViewer::init()
{
    osgDB::Registry::instance()->setDataFilePathList("/home/liuqijun/work/projects/osg-3.0.1/OpenSceneGraph-Data/");

    //init root and viewer
    m_root=new osg::Group;
    m_viewer=new osgViewer::Viewer;

    RenderState rs(m_root);
    rs.test();

    m_viewer->setSceneData(m_root);

}

void CoreViewer::start()
{
    m_viewer->run();
}
