//
//  AddToList.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/8/4.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "AddToList.h"
#import "AppDelegate.h"

extern void add_to_list(int new_value);
extern bool list_contains(int value_to_find);

@interface AddToList ()

@end

@implementation AddToList

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"AddToList";
    
    add_to_list(1);
    list_contains(1);
    
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
