//
//  GameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameScreen.h"
#include "GameScreenStates.h"
#include "Jon.h"

GameScreen::GameScreen(bool isLevelEditor) : m_fDeltaTime(0)
{
    m_touchPoint = std::unique_ptr<Vector2D>(new Vector2D());
    m_touchPointDown = std::unique_ptr<Vector2D>(new Vector2D());
    
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    
    m_game = std::unique_ptr<Game>(new Game());
    
    m_stateMachine = std::unique_ptr<StateMachine<GameScreen>>(new StateMachine<GameScreen>(this));
    
    if (isLevelEditor)
    {
        m_stateMachine->setCurrentState(LevelEditor::getInstance());
    }
    else
    {
        m_stateMachine->setCurrentState(GamePlay::getInstance());
    }
}

void GameScreen::init()
{
    m_game->load("{\"backgroundSkies\":[{\"x\":8,\"y\":19.9809,\"width\":16,\"height\":14.0381}],\"backgroundTrees\":[{\"x\":8,\"y\":14.9623,\"width\":16,\"height\":11.909}],\"backgroundCaves\":[{\"x\":8,\"y\":5.63865,\"width\":16,\"height\":11.2773}],\"trees\":[{\"x\":3.15789,\"y\":13.0789,\"width\":6.31579,\"height\":8.09532,\"treeType\":0},{\"x\":21.614,\"y\":14.1668,\"width\":11.2281,\"height\":10.2712,\"treeType\":1},{\"x\":35.9649,\"y\":13.8393,\"width\":7.92982,\"height\":9.61612,\"treeType\":2},{\"x\":83.1579,\"y\":13.0789,\"width\":6.31579,\"height\":8.09532,\"treeType\":0},{\"x\":101.614,\"y\":14.1668,\"width\":11.2281,\"height\":10.2712,\"treeType\":1},{\"x\":111.965,\"y\":13.8393,\"width\":7.92982,\"height\":9.61612,\"treeType\":2}],\"caveSkeletons\":[],\"grounds\":[{\"x\":0.760234,\"y\":5.11222,\"width\":1.52047,\"height\":10.2244,\"groundType\":3,\"boundsHeightFactor\":0.883295},{\"x\":25.4737,\"y\":5.11222,\"width\":47.9064,\"height\":10.2244,\"groundType\":2,\"boundsHeightFactor\":0.883295},{\"x\":50.2339,\"y\":5.11222,\"width\":1.61404,\"height\":10.2244,\"groundType\":6,\"boundsHeightFactor\":0.883295},{\"x\":66.7251,\"y\":8.90251,\"width\":1.73099,\"height\":2.64385,\"groundType\":7,\"boundsHeightFactor\":0.548673},{\"x\":91.5439,\"y\":8.90251,\"width\":47.9064,\"height\":2.64385,\"groundType\":0,\"boundsHeightFactor\":0.548673},{\"x\":116.304,\"y\":8.90251,\"width\":1.61404,\"height\":2.64385,\"groundType\":10,\"boundsHeightFactor\":0.548673},{\"x\":66.6433,\"y\":2.36309,\"width\":1.56725,\"height\":4.72617,\"groundType\":11,\"boundsHeightFactor\":0.257426},{\"x\":91.3801,\"y\":2.36309,\"width\":47.9064,\"height\":4.72617,\"groundType\":1,\"boundsHeightFactor\":0.257426},{\"x\":139.287,\"y\":2.36309,\"width\":47.9064,\"height\":4.72617,\"groundType\":1,\"boundsHeightFactor\":0.257426},{\"x\":164.012,\"y\":2.36309,\"width\":1.54386,\"height\":4.72617,\"groundType\":14,\"boundsHeightFactor\":0.257426},{\"x\":114.866,\"y\":3.17028,\"width\":2.19883,\"height\":6.34055,\"groundType\":15,\"boundsHeightFactor\":0.844444},{\"x\":127.953,\"y\":3.17028,\"width\":23.9766,\"height\":6.34055,\"groundType\":16,\"boundsHeightFactor\":0.844444},{\"x\":141.041,\"y\":3.17028,\"width\":2.19883,\"height\":6.34055,\"groundType\":18,\"boundsHeightFactor\":0.844444},{\"x\":168.433,\"y\":5.11222,\"width\":1.52047,\"height\":10.2244,\"groundType\":3,\"boundsHeightFactor\":0.883295},{\"x\":175.193,\"y\":5.11222,\"width\":12,\"height\":10.2244,\"groundType\":5,\"boundsHeightFactor\":0.883295},{\"x\":182,\"y\":5.11222,\"width\":1.61404,\"height\":10.2244,\"groundType\":6,\"boundsHeightFactor\":0.883295}],\"logVerticalTalls\":[{\"x\":16.8304,\"y\":10.0958,\"width\":1.66082,\"height\":2.12912},{\"x\":32.8304,\"y\":10.0958,\"width\":1.66082,\"height\":2.12912},{\"x\":44.8304,\"y\":10.0958,\"width\":1.66082,\"height\":2.12912}],\"logVerticalShorts\":[{\"x\":69.7602,\"y\":9.63951,\"width\":1.52047,\"height\":1.21664},{\"x\":48.7602,\"y\":9.63951,\"width\":1.52047,\"height\":1.21664}],\"thorns\":[{\"x\":81.4269,\"y\":9.7682,\"width\":2.8538,\"height\":1.474}],\"stumps\":[{\"x\":110.778,\"y\":10.7041,\"width\":3.92982,\"height\":3.34575}],\"sideSpikes\":[{\"x\":113.542,\"y\":3.6,\"width\":1.12281,\"height\":0.421144}],\"upwardSpikes\":[{\"x\":76.4795,\"y\":9.56932,\"width\":0.959064,\"height\":1.07626,\"upwardSpikeType\":1},{\"x\":78.3275,\"y\":9.54593,\"width\":0.654971,\"height\":1.02946,\"upwardSpikeType\":0},{\"x\":104.411,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":26.4795,\"y\":12.6894,\"width\":0.959064,\"height\":1.07626,\"upwardSpikeType\":1}],\"jumpSprings\":[{\"x\":43.0029,\"y\":9.53423,\"width\":1.00585,\"height\":1.00607,\"jumpSpringType\":0},{\"x\":147.608,\"y\":1.98873,\"width\":1.21637,\"height\":1.54419,\"jumpSpringType\":1},{\"x\":51.9181,\"y\":18.2864,\"width\":1.00585,\"height\":1.00607,\"jumpSpringType\":0}],\"rocks\":[{\"x\":88.0585,\"y\":10.2478,\"width\":4.49123,\"height\":4.30503,\"isCracked\":false},{\"x\":92.0585,\"y\":10.2478,\"width\":4.49123,\"height\":4.30503,\"isCracked\":true}],\"platforms\":[{\"x\":24.3509,\"y\":11.8312,\"width\":0.701754,\"height\":1.16984,\"groundPlatformType\":1},{\"x\":26.0585,\"y\":11.8312,\"width\":2.71345,\"height\":1.16984,\"groundPlatformType\":2},{\"x\":27.7661,\"y\":11.8312,\"width\":0.701754,\"height\":1.16984,\"groundPlatformType\":3},{\"x\":32.3509,\"y\":14.6312,\"width\":0.701754,\"height\":1.16984,\"groundPlatformType\":1},{\"x\":34.0585,\"y\":14.6312,\"width\":2.71345,\"height\":1.16984,\"groundPlatformType\":2},{\"x\":36.7719,\"y\":14.6312,\"width\":2.71345,\"height\":1.16984,\"groundPlatformType\":2},{\"x\":38.4795,\"y\":14.6312,\"width\":0.701754,\"height\":1.16984,\"groundPlatformType\":3},{\"x\":48.3509,\"y\":17.4312,\"width\":0.701754,\"height\":1.16984,\"groundPlatformType\":1},{\"x\":50.0585,\"y\":17.4312,\"width\":2.71345,\"height\":1.16984,\"groundPlatformType\":2},{\"x\":52.7719,\"y\":17.4312,\"width\":2.71345,\"height\":1.16984,\"groundPlatformType\":2},{\"x\":54.4795,\"y\":17.4312,\"width\":0.701754,\"height\":1.16984,\"groundPlatformType\":3},{\"x\":96.2105,\"y\":4,\"width\":0.795322,\"height\":1.16984,\"groundPlatformType\":4},{\"x\":97.9649,\"y\":4,\"width\":2.71345,\"height\":1.16984,\"groundPlatformType\":5},{\"x\":99.7076,\"y\":4,\"width\":0.77193,\"height\":1.16984,\"groundPlatformType\":6},{\"x\":156.023,\"y\":4.4,\"width\":0.795322,\"height\":1.16984,\"groundPlatformType\":4},{\"x\":157.778,\"y\":4.4,\"width\":2.71345,\"height\":1.16984,\"groundPlatformType\":5},{\"x\":159.52,\"y\":4.4,\"width\":0.77193,\"height\":1.16984,\"groundPlatformType\":6}],\"endSigns\":[{\"x\":180.795,\"y\":9.59272,\"width\":0.795322,\"height\":1.12305}],\"carrots\":[{\"x\":14.6082,\"y\":10.2312,\"width\":1.21637,\"height\":0.959272},{\"x\":24.6082,\"y\":10.2312,\"width\":1.21637,\"height\":0.959272},{\"x\":40.6082,\"y\":10.2312,\"width\":1.21637,\"height\":0.959272},{\"x\":96.4211,\"y\":10.2312,\"width\":1.21637,\"height\":0.959272},{\"x\":99.4211,\"y\":10.2312,\"width\":1.21637,\"height\":0.959272},{\"x\":102.421,\"y\":10.2312,\"width\":1.21637,\"height\":0.959272},{\"x\":172.234,\"y\":10.2312,\"width\":1.21637,\"height\":0.959272},{\"x\":175.234,\"y\":12.2312,\"width\":1.21637,\"height\":0.959272},{\"x\":178.234,\"y\":15.2312,\"width\":1.21637,\"height\":0.959272}],\"goldenCarrots\":[{\"x\":55.6082,\"y\":23.0312,\"width\":1.21637,\"height\":1.14645},{\"x\":155.234,\"y\":2.6,\"width\":1.21637,\"height\":1.14645}],\"jons\":[{\"x\":3.2,\"y\":10.1312,\"width\":2.2,\"height\":2.2}]}");
    
    m_renderer->init();
}

void GameScreen::onResume()
{
    // TODO
}

void GameScreen::onPause()
{
    // TODO
}

void GameScreen::update(float deltaTime)
{
    m_fDeltaTime = deltaTime;
    
    m_stateMachine->execute();
}

void GameScreen::render()
{
    m_renderer->render(*m_game);
}

int GameScreen::getState()
{
	return 0;
}

void GameScreen::clearState()
{
	// TODO
}

short GameScreen::getCurrentMusicId()
{
    short musicId = Assets::getInstance()->getMusicId();
    Assets::getInstance()->setMusicId(0);
    
    return musicId;
}

short GameScreen::getCurrentSoundId()
{
    short playThisSound = Assets::getInstance()->getFirstSoundId();
    Assets::getInstance()->eraseFirstSoundId();
    
    return playThisSound;
}

void GameScreen::onTouch(Touch_Type type, float raw_touch_x, float raw_touch_y)
{
    if (type == Touch_Type::DRAGGED && m_touchEventsBuffer.size() > 3)
    {
        return;
    }
    
    addTouchEventForType(type, raw_touch_x, raw_touch_y);
}

void GameScreen::processTouchEvents()
{
    for (std::vector<TouchEvent>::iterator i = m_touchEvents.begin(); i != m_touchEvents.end(); i++)
    {
        if(m_touchEventsPool.size() < 50)
        {
            m_touchEventsPool.push_back(*i);
        }
    }
    
    m_touchEvents.clear();
    m_touchEvents.swap(m_touchEventsBuffer);
    m_touchEventsBuffer.clear();
}

#pragma mark private

TouchEvent GameScreen::newTouchEvent()
{
    if (m_touchEventsPool.size() == 0)
    {
        return TouchEvent(0, 0, Touch_Type::DOWN);
    }
    else
    {
        TouchEvent touchEvent = m_touchEventsPool.back();
        m_touchEventsPool.pop_back();
        
        return touchEvent;
    }
}

void GameScreen::addTouchEventForType(Touch_Type touchType, float x, float y)
{
    TouchEvent touchEvent = newTouchEvent();
    touchEvent.setTouchType(touchType);
    touchEvent.setX(x);
    touchEvent.setY(y);
    
    m_touchEventsBuffer.push_back(touchEvent);
}