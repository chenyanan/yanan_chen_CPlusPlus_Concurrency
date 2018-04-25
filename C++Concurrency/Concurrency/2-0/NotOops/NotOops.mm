//
//  NotOops.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/6/24.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "NotOops.h"

extern void oops(int some_param);
extern void not_oops(int some_param);

@interface NotOops ()

@end

@implementation NotOops

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"NotOops";
    
    oops(100);
    not_oops(100);
    
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
