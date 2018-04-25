//
//  FunctionCallFunctionUseThreadAsParameter.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/6/24.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "FunctionCallFunctionUseThreadAsParameter.h"

extern void function_call_function_use_thread_as_parameter();

@interface FunctionCallFunctionUseThreadAsParameter ()

@end

@implementation FunctionCallFunctionUseThreadAsParameter

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"FunctionCallFunctionUseThreadAsParameter";
    
    function_call_function_use_thread_as_parameter();
    
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
