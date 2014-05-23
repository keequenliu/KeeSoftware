#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include <osgViewer/Viewer>
#include <osg/Group>

using namespace osg;


/**
  *此类只用于测试OSG的渲染状态，渲染状态一共有：
  * 固定管线，Alpha检验，Blending融合，颜色蒙板，face Culling，深度检测，雾，点和线的光栅化，顶点着色器，片段着色器
  * osg通过StateSet来设置渲染状态，它一般需要关联至场景节点，或关联至osg::Drawable类，它能自动优化冗余的渲染状态。
  *
  **/
class RenderState
{
public:
    RenderState(osg::Group* root);

    //单色 背面剔除 线框模式
    void test();
    void textureTest();
    //多个二维纹理的叠加（多重纹理）
    void mutiTextureTest();
    void mipmapTextureTest();


private:
    osg::ref_ptr<Group> m_scene; //测试场景
};


#endif // RENDERSTATE_H
