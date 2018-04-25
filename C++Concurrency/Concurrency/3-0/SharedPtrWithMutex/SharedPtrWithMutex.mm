//
//  SharedPtrWithMutex.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/8/4.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "SharedPtrWithMutex.h"
#import "AppDelegate.h"

class my_class {};

extern void init_or_get_resource();
extern void init_or_get_resource_with_lock();
extern void undefined_behaviour_with_double_checked_locking();
extern void init_resource_with_call_one_and_once_flag();
extern my_class& get_my_class_instance();

@interface SharedPtrWithMutex ()

@end

@implementation SharedPtrWithMutex

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"SharedPtrWithMutex";
    
    init_or_get_resource();
    init_or_get_resource_with_lock();
    undefined_behaviour_with_double_checked_locking();
    init_resource_with_call_one_and_once_flag();
    get_my_class_instance();
    
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
