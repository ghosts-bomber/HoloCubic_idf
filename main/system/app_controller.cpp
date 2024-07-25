#include "app_controller.h"

AppController::AppController() {}
AppController::~AppController() {}
void AppController::init() {}
int AppController::app_install(APP_OBJ *app)
{
    if (!app)
        return -1;
    app_list_[app_num_++] = app;
    if(cur_app_index_<0) cur_app_index_ = 0;
    return 0;
}
int AppController::app_uninstall(const APP_OBJ *app) { return 0; }

void AppController::enter_app(const APP_OBJ *app)
{
    if (app && app->app_init)
    {
        (app->app_init)(this);
        menu_ = false;
    }
}
void AppController::app_exit() {}

void AppController::process_control(const ImuAction *action)
{
    if(menu_){

    }else{
        auto app = cur_app();
        if(app && app->main_process){
            (app->main_process)(this,action);
        }
    }
}

APP_OBJ *AppController::next_app()
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
APP_OBJ *AppController::pre_app()
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
APP_OBJ* AppController::cur_app()
{
    if(app_num_==0){
        return nullptr;
    }else{
        return app_list_[cur_app_index_];
    }

}