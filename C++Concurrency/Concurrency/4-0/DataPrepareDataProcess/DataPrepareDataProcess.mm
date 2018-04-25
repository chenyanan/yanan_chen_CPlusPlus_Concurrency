//
//  DataPrepareDataProcess.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/8/4.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "DataPrepareDataProcess.h"
#include <thread>

extern void data_preparation_thread();
extern void data_processing_thread();

@interface DataPrepareDataProcess ()

@end

@implementation DataPrepareDataProcess

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"DataPrepareDataProcess";
    
    std::thread a(data_preparation_thread);
    std::thread b(data_processing_thread);
    
    if (a.joinable())
        a.detach();
    
    if (b.joinable())
        b.detach();
    
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
