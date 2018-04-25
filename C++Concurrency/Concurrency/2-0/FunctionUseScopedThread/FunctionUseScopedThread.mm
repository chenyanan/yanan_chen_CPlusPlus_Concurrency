//
//  FunctionUseScopedThread.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/6/24.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "FunctionUseScopedThread.h"

extern void function_use_scoped_thread();

@interface FunctionUseScopedThread ()

@end

@implementation FunctionUseScopedThread

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"FunctionUseScopedThread";
    
    function_use_scoped_thread();
    
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
