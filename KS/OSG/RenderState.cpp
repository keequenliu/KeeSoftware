#include "RenderState.h"

#include <osg/PositionAttitudeTransform>
#include <osg/ShadeModel>
#include <osg/CullFace>
#include <osg/PolygonMode>
#include <osgDB/ReadFile>
#include <osg/Texture2D>

osg::Geometry* createQuadGeometry(osg::Vec3 pos=osg::Vec3(0,0,0)
                                  ,osg::Vec3 widthVec=osg::Vec3(100,0,0)
                                  ,osg::Vec3 heigthVec=osg::Vec3(0,0,100),bool color=true)
{
    osg::Geometry* geom=new osg::Geometry;
    osg::Vec3Array* verArray=new osg::Vec3Array();
    verArray->push_back(pos);
    verArray->push_back(pos+widthVec);
    verArray->push_back(widthVec+heigthVec);
    verArray->push_back(heigthVec);

    osg::Vec4Array* colorArray=new osg::Vec4Array;
    if(color)
    {
        colorArray->push_back(osg::Vec4(1,0,0,1));
        colorArray->push_back(osg::Vec4(0,1,0,1));
        colorArray->push_back(osg::Vec4(0,0,1,1));
        colorArray->push_back(osg::Vec4(1,1,1,1));
        geom->setColorArray(colorArray);
        geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    }
    else
    {
        colorArray->push_back(osg::Vec4(1,1,1,1));
        geom->setColorArray(colorArray);
        geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
    }

    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(1);
    (*normals)[0]=widthVec^heigthVec;
    (*normals)[0].normalize();

    geom->setVertexArray(verArray);

    geom->setNormalArray(normals);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

    return geom;
}


osg::Geometry* createTextureGeometry(osg::Vec3 pos=osg::Vec3(0,0,0)
                                     ,osg::Vec3 widthVec=osg::Vec3(100,0,0)
                                     ,osg::Vec3 heigthVec=osg::Vec3(0,0,100))
{
    osg::Geometry* geom=createQuadGeometry(pos,widthVec,heigthVec,false);

    osg::Vec2Array* tcoords=new osg::Vec2Array();
    tcoords->push_back(osg::Vec2(0,0));
    tcoords->push_back(osg::Vec2(1,0));
    tcoords->push_back(osg::Vec2(1,1));
    tcoords->push_back(osg::Vec2(0,1));
    //    (*tcoords)[0].set(0,1);
    //    (*tcoords)[1].set(0,0);
    //    (*tcoords)[2].set(1,0);
    //    (*tcoords)[3].set(1,1);

    geom->setTexCoordArray(0,tcoords);

    return geom;
}


RenderState::RenderState(osg::Group* root)
{
    m_scene=root;
}

void RenderState::test()
{
    //模式为openGl glEnable对应的参数

    //默认禁用灯光
    m_scene->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF|osg::StateAttribute::PROTECTED); //protected，不受父节点影响

    //create top left 默认
    {
        osg::Geode * geode=new osg::Geode;
        osg::Geometry* geo=createQuadGeometry();
        geode->addDrawable(geo);

        osg::ref_ptr<osg::PositionAttitudeTransform> pat=new osg::PositionAttitudeTransform;
        pat->setPosition(osg::Vec3(-100,0,0));
        pat->addChild(geode);

        m_scene->addChild(pat);
    }
    //create top bottom 单色
    {
        osg::Geode * geode=new osg::Geode;
        osg::Geometry* geo=createQuadGeometry();
        geode->addDrawable(geo);

        osg::ref_ptr<osg::PositionAttitudeTransform> pat=new osg::PositionAttitudeTransform;
        pat->setPosition(osg::Vec3(-100,0,-105));
        pat->addChild(geode);
        //注意这里，设置的是PAT的状态集。
        pat->getOrCreateStateSet()->setAttribute(new osg::ShadeModel(osg::ShadeModel::FLAT)); //单色

        m_scene->addChild(pat);
    }

    //create right bottom 背面剔除
    {
        osg::Geode * geode=new osg::Geode;
        osg::Geometry* geo=createQuadGeometry();
        geode->addDrawable(geo);

        osg::ref_ptr<osg::PositionAttitudeTransform> pat=new osg::PositionAttitudeTransform;
        pat->setPosition(osg::Vec3(5,0,0));
        pat->addChild(geode);
        //背面剔除
        pat->getOrCreateStateSet()->setAttributeAndModes(new osg::CullFace);

        m_scene->addChild(pat);
    }

    //create right top 线框模式
    {
        osg::Geode * geode=new osg::Geode;
        osg::Geometry* geo=createQuadGeometry();
        geode->addDrawable(geo);

        osg::ref_ptr<osg::PositionAttitudeTransform> pat=new osg::PositionAttitudeTransform;
        pat->setPosition(osg::Vec3(5,0,-105));
        pat->addChild(geode);

        osg::PolygonMode* pm = new osg::PolygonMode(osg::PolygonMode::FRONT,osg::PolygonMode::LINE);
        pat->getOrCreateStateSet()->setAttributeAndModes(pm);

        m_scene->addChild(pat);
    }

    //纹理
    {
        osg::Geode * geode=new osg::Geode;
        osg::Geometry* geom=createTextureGeometry(osg::Vec3(0,0,0),osg::Vec3(200,0,0),osg::Vec3(0,0,200));
        osg::Texture2D* texture=new osg::Texture2D(osgDB::readImageFile("Images/osgshaders1.png"));
        texture->setResizeNonPowerOfTwoHint(false);
        texture->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
        texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR);
        texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
        texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);

        texture->setUnRefImageDataAfterApply(true);//创建openGL纹理对象后，删除image对象

        //这里一定要注意是setTextureAttributeAndModes。
        geom->getOrCreateStateSet()->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
        geode->addDrawable(geom);

        osg::ref_ptr<osg::PositionAttitudeTransform> pat=new osg::PositionAttitudeTransform;
        pat->setPosition(osg::Vec3(110,0,-105));
        pat->addChild(geode);

        //纹理设置线框后，纹理不显示
        //        osg::PolygonMode* pm = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
        //        pat->getOrCreateStateSet()->setAttributeAndModes(pm);

        m_scene->addChild(pat);
        //纹理的宽度没有设置成功
        //        texture->setTextureWidth( 10 );
        //        texture->setTextureHeight( 10 );
        //        texture->setInternalFormat( GL_RGBA );
        //        texture->setBorderWidth( 3 );
        //        texture->setBorderColor(osg::Vec4(1,0,0,1));
        //        texture->setBorderWidth(3);

    }
}
