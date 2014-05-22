#ifndef COREVIEWER_H
#define COREVIEWER_H

#include <osgViewer/Viewer>
#include <osg/Group>

//用于组织整个场景与视口
class CoreViewer
{
public:
    CoreViewer();
    ~CoreViewer();

    void init();
    void start();

    osg::Geometry* createDrawable(int w=1,int h=1);
private:
    osg::ref_ptr<osgViewer::Viewer> m_viewer;
    osg::ref_ptr<osg::Group> m_root;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_cowPAT;
};

#endif // COREVIEWER_H
