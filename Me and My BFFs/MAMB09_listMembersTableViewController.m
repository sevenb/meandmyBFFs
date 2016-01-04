//
//  MAMB09_listMembersTableViewController.m
//  Me&myBFFs
//
//  Created by Richard Koskela on 2016-01-04.
//  Copyright © 2016 Richard Koskela. All rights reserved.
//

#import "MAMB09_listMembersTableViewController.h"
#import "mamblib.h"
#import "MAMB09AppDelegate.h"   // to get globals

@interface MAMB09_listMembersTableViewController ()

@end

@implementation MAMB09_listMembersTableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
    
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
tn();
    NSLog(@"in LIST MEMBEFRS   viewDidLoad!");

//    [self.tableView setBackgroundColor: gbl_colorReportsBG];
    [self.tableView setBackgroundColor: gbl_colorEditingBG ];

    [gbl_arrayMembersToPickFrom removeAllObjects];
    gbl_arrayMembersToPickFrom = [[NSMutableArray alloc] init];

    // INCLUDE ONLY   members of gbl_lastSelectedGroup
    //
    for (id myMemberRec in gbl_arrayMem) {

// skip example record  TODO in production
//            if (gbl_show_example_data ==  NO  &&
//                [mymyMemberRecPSV hasPrefix: @"~"]) {  // skip example record
//                continue;         //  ======================-------------------------------------- PUT BACK when we have non-example data!!!
//            }
//

        NSArray *psvArray;
        NSString *currGroup;
        NSString *currMember;
        
        psvArray = [myMemberRec componentsSeparatedByCharactersInSet: [NSCharacterSet characterSetWithCharactersInString:@"|"]];
        currGroup  = psvArray[0];
        currMember = psvArray[1];

        if ([currGroup isEqualToString: gbl_lastSelectedGroup ] )
        {
            [gbl_arrayMembersToPickFrom addObject: currMember ];                        //  Person name for pick
        }
    } // for each groupmember

 NSLog(@"gbl_arrayMembersToPickFrom=%@",gbl_arrayMembersToPickFrom);
 NSLog(@"gbl_arrayMembersToPickFrom.count=%lu",(unsigned long)gbl_arrayMembersToPickFrom.count);

} // viewDidLoad



- (void) viewWillAppear: (BOOL) animated
{
    [super viewWillAppear: animated];
  NSLog(@"in viewWillAppear! in LIST MEMBERS ");


    // use 2 buttons in a toolbar on bottom of screen to  add- green "+"  delete- red "-"
    //
    UIBarButtonItem *myPrompt = [[UIBarButtonItem alloc]initWithTitle: @"Members" 
                                                                style: UIBarButtonItemStylePlain
                                                               target: self
                                                               action: nil
    ];

    [myPrompt setTitleTextAttributes: @{
//                    NSFontAttributeName: [UIFont fontWithName:@"Helvetica-Bold" size:26.0],
//                    NSFontAttributeName: [UIFont boldSystemFontOfSize: 17.0],
                    NSFontAttributeName: [UIFont boldSystemFontOfSize: 20.0],
//         NSForegroundColorAttributeName: [UIColor greenColor]
         NSForegroundColorAttributeName: [UIColor blackColor]
       }
                            forState: UIControlStateNormal
    ];



    UIImage *myImageADD = [[UIImage imageNamed: @"iconPlusAddGreenBig_66.png"]
                        imageWithRenderingMode: UIImageRenderingModeAlwaysOriginal ];
    UIImage *myImageDEL = [[UIImage imageNamed: @"iconMinusDelRedBig_66.png"]
                        imageWithRenderingMode: UIImageRenderingModeAlwaysOriginal ];


//    UIBarButtonItem *memberADD = [[UIBarButtonItem alloc] initWithImage: [UIImage imageNamed: @"iconPlusAddGreenBig_66.png"]
    UIBarButtonItem *memberADD = [[UIBarButtonItem alloc] initWithImage: myImageADD
                                                                  style: UIBarButtonItemStylePlain
                                                                 target: self
                                                                 action: @selector(pressedGreenAdd)
    ];
//    UIBarButtonItem *memberDEL = [[UIBarButtonItem alloc] initWithImage: [UIImage imageNamed: @"iconMinusDelRedBig_66.png"]
    UIBarButtonItem *memberDEL = [[UIBarButtonItem alloc] initWithImage: myImageDEL
                                                                  style: UIBarButtonItemStylePlain
                                                                 target: self
                                                                 action: @selector(pressedRedDelete)
    ];

    
    UIBarButtonItem *myFlexibleSpace = [[UIBarButtonItem alloc] initWithBarButtonSystemItem: UIBarButtonSystemItemFlexibleSpace
                                                                                     target: self
                                                                                     action: nil];
        // create a Toolbar

//       UIToolbar *myToolbar = [[UIToolbar alloc] initWithFrame:CGRectMake(0, self.view.frame.size.height - 44, 320, 44)];
//       UIToolbar *myToolbar = [[UIToolbar alloc] initWithFrame:CGRectMake(0, self.view.frame.size.height - 44, self.view.frame.size.width, 44)];
//        gbl_toolbarForwBack = [[UIToolbar alloc] initWithFrame:CGRectMake(0, self.view.frame.size.height - 44, self.view.frame.size.width, 44)];

    float my_screen_height;
    float my_status_bar_height;
    float my_nav_bar_height;
    float my_toolbar_height;

    MAMB09AppDelegate *myappDelegate = (MAMB09AppDelegate *)[[UIApplication sharedApplication] delegate]; // for gbl methods in appDelegate.m
    CGSize currentScreenWidthHeight = [myappDelegate currentScreenSize];
    my_screen_height = currentScreenWidthHeight.height;

    CGSize myStatusBarSize = [[UIApplication sharedApplication] statusBarFrame].size;
    my_status_bar_height   = MIN(myStatusBarSize.width, myStatusBarSize.height);

    my_nav_bar_height    =  self.navigationController.navigationBar.frame.size.height;


//  NSLog(@"cu33entScreenWidthHeight.width  =%f",currentScreenWidthHeight.width );
//  NSLog(@"cu33entScreenWidthHeight.height =%f",currentScreenWidthHeight.height );
  NSLog(@"my_screen_height                  =%f",my_screen_height );
  NSLog(@"my_status_bar_height              =%f",my_status_bar_height   );
  NSLog(@"my_nav_bar_height                 =%f",my_nav_bar_height    );
  NSLog(@"my_toolbar_height                 =%f",my_toolbar_height );

    my_toolbar_height = 44.0;

    float y_value_of_toolbar; 
    //    y_value_of_toolbar  = currentScreenWidthHeight.height - 44.0;
    //    y_value_of_toolbar  = 400.0;
    //    y_value_of_toolbar  = 436.0;
    //    y_value_of_toolbar  = 480.0;
    //    y_value_of_toolbar  = 472.0;// too low
    //    y_value_of_toolbar  = 464.0; // very close
    //    y_value_of_toolbar  = 458.0; // too high
    //    y_value_of_toolbar  = 456.0; // too high
    //    y_value_of_toolbar  = 459.0; // too high
    //    y_value_of_toolbar  = 460.0; // very close   exact
    y_value_of_toolbar  = my_screen_height  - my_status_bar_height  - my_nav_bar_height - my_toolbar_height;
  NSLog(@"y_value_of_toolbar  =%f",y_value_of_toolbar  );


    gbl_toolbarMemberAddDel = [[UIToolbar alloc] initWithFrame:CGRectMake(
        0.0,
        y_value_of_toolbar, 
        currentScreenWidthHeight.width,
        44.0)];


//
//439://      gbl_myCellBgView =[[UIView alloc] initWithFrame:CGRectMake(0.0f, 0.0f, [cell frame].size.width -20, [cell frame].size.height)];
//906:// CGRect pickerFrame = CGRectMake(0.0, viewFrame.size.height-pickerHeight, viewFrame.size.width, pickerHeight);
//28:    self.outletWebView.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height);
//

        // make array of buttons for the Toolbar
        NSArray *myButtonArray =  [NSArray arrayWithObjects:
//            myFlexibleSpace, myPrompt, myFlexibleSpace, memberADD, myFlexibleSpace, memberDEL, myFlexibleSpace, nil
//            myFlexibleSpace, myFlexibleSpace, myPrompt, myFlexibleSpace, memberADD, myFlexibleSpace, memberDEL, myFlexibleSpace, myFlexibleSpace, nil
            myFlexibleSpace, myFlexibleSpace, myFlexibleSpace,
            myPrompt, myFlexibleSpace, memberADD, myFlexibleSpace, memberDEL,
            myFlexibleSpace, myFlexibleSpace, myFlexibleSpace, nil
        ]; 

        // put the array of buttons in the Toolbar
        [gbl_toolbarMemberAddDel setItems: myButtonArray   animated: NO];
nbn(129);
            // set bottom toolbar bg color to white
            //
            gbl_toolbarMemberAddDel.translucent  = NO;
            gbl_toolbarMemberAddDel.barTintColor = [UIColor whiteColor];

        // put the Toolbar onto bottom of what color view
        dispatch_async(dispatch_get_main_queue(), ^(void){
//             self.navigationController.toolbar.hidden = YES;
            [self.view addSubview: gbl_toolbarMemberAddDel ];

//            [self.navigationController.view addSubview: gbl_toolbarForwBack ];  // this worked  but in info, it stayed  also allows too fast
//             self.navigationController.toolbar.hidden = NO;
//            [self.navigationController.toolbar setItems: myButtonArray ]; 
//            self.navigationController.toolbar.items = myButtonArray; 
//             self.navigationController.toolbar.hidden = NO

        });


} // end of   viewWillAppear


- (IBAction)pressedGreenAdd
{
  NSLog(@"in pressedGreenAdd");

    dispatch_async(dispatch_get_main_queue(), ^{                                 // <===  
        [self performSegueWithIdentifier:@"segueListMembersToSelNewMembers" sender:self]; //  
    });

} // (IBAction)pressedGreenAdd


- (IBAction)pressedRedDel
{
  NSLog(@"in pressedRedDel");
} // (IBAction)pressedRedDel


- (void)didReceiveMemoryWarning {
    //    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
    UIAlertController* alert = [UIAlertController alertControllerWithTitle: @"The App received a Memory Warning"
                                                                   message: @"The system has determined that the \namount of available memory is very low."
                                                            preferredStyle: UIAlertControllerStyleAlert  ];
     
    UIAlertAction*  okButton = [UIAlertAction actionWithTitle: @"OK"
                                                        style: UIAlertActionStyleDefault
                                                      handler: ^(UIAlertAction * action) {
        NSLog(@"Ok button pressed");
    } ];
    [alert addAction:  okButton];
    [self presentViewController: alert  animated: YES  completion: nil   ];
    [super didReceiveMemoryWarning];
} // didReceiveMemoryWarning 



#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return gbl_arrayMembersToPickFrom.count;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
tn();
  NSLog(@"in cellForRowAtIndexPath");
  NSLog(@"=[%ld]",(long)indexPath.row);
    //    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:<#@"reuseIdentifier"#> forIndexPath:indexPath];

    // create an NSString  we can use as the reuse identifier
    static NSString *CellIdentifier = @"SelPersonCellIdentifier";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier: CellIdentifier];
    // if there are no cells to be reused, create a new cell
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    // Configure the cell...


    UIFont *myNewFont =  [UIFont boldSystemFontOfSize: 17.0];

    cell.textLabel.text = [gbl_arrayMembersToPickFrom   objectAtIndex:indexPath.row];
    //        cell.textLabel.font = [UIFont systemFontOfSize: 16.0];
    cell.textLabel.font = myNewFont;
    
  NSLog(@"cell.textLabel.text=[%@]",cell.textLabel.text);
    return cell;
}

// color cell bg
- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath {
    
    [self.tableView setBackgroundView:nil];

    // UIColor *gbl_colorEditingBG_current;  // is now yellow or blue for add a record screen  (addChange view)
    cell.backgroundColor = [UIColor clearColor];
}

// how to set the tableview cell height
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath  // -------------------------
{
//  NSLog(@"in heightForRowAtIndexPath 1");

  return 44.0; // matches report height
}


/*
//// Override to support conditional editing of the table view.
//- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
//    // Return NO if you do not want the specified item to be editable.
//    return YES;
//}
*/

/*
// Override to support editing the table view.
//- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
//    if (editingStyle == UITableViewCellEditingStyleDelete) {
//        // Delete the row from the data source
//        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
//    } else if (editingStyle == UITableViewCellEditingStyleInsert) {
//        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
//    }   
//}
*/

/*
// Override to support rearranging the table view.
//- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
//}
*/

/*
// Override to support conditional rearranging of the table view.
//- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
//    // Return NO if you do not want the item to be re-orderable.
//    return YES;
//}
*/


#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {

  NSLog(@"in prepareForSegue  in LIST MEMbers");
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}


@end