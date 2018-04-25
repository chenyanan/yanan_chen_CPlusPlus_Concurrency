//
//  ThreadDetachUseLocalObject.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/6/24.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "ThreadDetachUseSomeLocalObject.h"

extern void thread_detach_use_some_local_object();

@interface ThreadDetachUseSomeLocalObject ()

@end

@implementation ThreadDetachUseSomeLocalObject

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"ThreadDetachUseLocalObject";
    
    thread_detach_use_some_local_object();
    
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
