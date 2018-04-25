//
//  RaceConditionComeIfOperationUnprotectedByALock.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/8/4.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "RaceConditionComeIfOperationUnprotectedByALock.h"
#import "AppDelegate.h"

extern void race_condition_come_if_operation_unprotected_by_a_lock();

@interface RaceConditionComeIfOperationUnprotectedByALock ()

@end

@implementation RaceConditionComeIfOperationUnprotectedByALock

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"RaceConditionComeIfOperationUnprotectedByALock";
    
    race_condition_come_if_operation_unprotected_by_a_lock();
    
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
