#include "app_controller.h"

AppContorller::AppContorller() {}
AppContorller::~AppContorller() {}
void AppContorller::init() {}
int AppContorller::app_install(APP_OBJ *app)
{
    if (!app)
        return -1;
    app_list_[app_num_++] = app;
    if(cur_app_index_<0) cur_app_index_ = 0;
    return 0;
}
int AppContorller::app_uninstall(const APP_OBJ *app) { return 0; }

void AppContorller::enter_app(const APP_OBJ *app)
{
    if (app && app->app_init)
    {
        (app->app_init)();
        menu_ = false;
    }
}
void AppContorller::app_exit() {}

void AppContorller::process_control(const ControlAction *action)
{
    if(menu_){

    }else{
        auto app = cur_app();
        if(app && app->process_action){
            (app->process_action)(action);
        }
    }
}

APP_OBJ *AppContorller::next_app()
{
    if (app_num_ == 0)
    {
        return nullptr;
    }
    else
    {
        if (cur_app_index_ == app_num_ - 1)
        {
            cur_app_index_ = 0;
        }
        else
        {
            cur_app_index_++;
        }
        return app_list_[cur_app_index_];
    }
}
APP_OBJ *AppContorller::pre_app()
{
    if (app_num_ == 0)
    {
        return nullptr;
    }
    else
    {
        if (cur_app_index_ == 0)
        {
            cur_app_index_ = app_num_ - 1;
        }
        else
        {
            cur_app_index_--;
        }
        return app_list_[cur_app_index_];
    }
}
APP_OBJ* AppContorller::cur_app()
{
    if(app_num_==0){
        return nullptr;
    }else{
        return app_list_[cur_app_index_];
    }

}