//
//  Clock.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/8/6.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "Clock.h"
#import "AppDelegate.h"

extern void milliseconds_to_seconds();
extern void future_wait_for_and_do_something();
extern void get_time_interval();
extern bool wait_loop();

@interface Clock ()

@end

@implementation Clock

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"Clock";
    
    milliseconds_to_seconds();
    future_wait_for_and_do_something();
    get_time_interval();
    wait_loop();
    
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
