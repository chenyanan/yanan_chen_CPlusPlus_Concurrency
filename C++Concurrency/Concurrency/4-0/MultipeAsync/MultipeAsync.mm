//
//  MultipeAsync.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/8/6.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "MultipeAsync.h"
#import "AppDelegate.h"

extern void async_returen_value_and_unique_ptr_parameter();
extern void multiple_async();

@interface MultipeAsync ()

@end

@implementation MultipeAsync

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"MultipeAsync";
    
    async_returen_value_and_unique_ptr_parameter();
    multiple_async();
    
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
