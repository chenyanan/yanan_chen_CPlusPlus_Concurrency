//
//  SwapObjectWithLockGuard.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/8/4.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "SwapObjectWithLockGuard.h"
#import "AppDelegate.h"

extern void swap_object_with_lock_guard();

@interface SwapObjectWithLockGuard ()

@end

@implementation SwapObjectWithLockGuard

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"SwapObjectWithLockGuard";
    
    swap_object_with_lock_guard();
    
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
