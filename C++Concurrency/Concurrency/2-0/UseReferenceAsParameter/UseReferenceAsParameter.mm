//
//  UseReferenceAsParameter.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/7/11.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "UseReferenceAsParameter.h"
#import "AppDelegate.h"

class X;

@interface UseReferenceAsParameter ()

@end

@implementation UseReferenceAsParameter

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"UseReferenceAsParameter";
    
    X * x_ptr;
    x_ptr = NULL;
    NSLog(@"go to see class X");
    
/*
 
    class X
    {
    public:
        void do_lengthy_work() { std::cout << "X::do_lengthy_work()" << std::endl; }
        void do_lengthy_work_with_int(int) { std::cout << "do_lengthy_work_with_int(int)" << std::endl; }
    };

    X my_x;
    std::thread t_0(&X::do_lengthy_work, &my_x);   //①

    int num(0);
    std::thread t_1(&X::do_lengthy_work_with_int, &my_x, num);
 
*/
    
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
