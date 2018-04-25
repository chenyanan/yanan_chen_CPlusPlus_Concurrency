//
//  SomeThreadMoveOperations.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/6/24.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "SomeThreadMoveOperations.h"

extern void some_thread_move_operations();

@interface SomeThreadMoveOperations ()

@end

@implementation SomeThreadMoveOperations

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"SomeThreadMoveOperations";
    
    some_thread_move_operations();
    
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
