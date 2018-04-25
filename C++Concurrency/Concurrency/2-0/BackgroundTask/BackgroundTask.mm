//
//  BackgroundTask.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/7/11.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "BackgroundTask.h"
#import "AppDelegate.h"

class background_task;

@interface BackgroundTask ()

@end

@implementation BackgroundTask

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"BackgroundTask";
    
    background_task * task_ptr;
    task_ptr = NULL;
    NSLog(@"go to see background_task");
    
/*
     
     class background_task
     {
     public:
         void operator()() const
         {
             do_something();
             do_something_else();
         }
     };
     
     background_task f;
     std::thread my_thread_2(f);
     std::thread not_thread(background_task());   //声明了一个函数而不是声明了一个std::thread
     std::thread my_thread_3((background_task()));   //正确声明了一个std::thread
     std::thread my_thread_4{background_task()};   //正确声明了一个std::thread
     std::thread my_thread_5([]{
         do_something();
         do_something_else();
     });
     
*/
    
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
