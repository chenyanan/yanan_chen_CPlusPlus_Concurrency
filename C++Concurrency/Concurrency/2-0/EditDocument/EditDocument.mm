//
//  EditDocument.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/6/24.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "EditDocument.h"

#include <string>
extern void edit_document(const std::string& filename);
extern std::string get_filename_from_user();

@interface EditDocument ()

@end

@implementation EditDocument

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"EditDocument";
    
    edit_document(get_filename_from_user());
    
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
