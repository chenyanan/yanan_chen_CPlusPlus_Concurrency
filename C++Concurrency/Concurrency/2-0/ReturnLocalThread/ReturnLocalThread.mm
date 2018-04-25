//
//  ReturnLocalThread.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/6/24.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "ReturnLocalThread.h"

#include <thread>
extern std::thread return_local_thread();

@interface ReturnLocalThread ()

@end

@implementation ReturnLocalThread

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"ReturnLocalThread";
    
    return_local_thread().join();
    
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
