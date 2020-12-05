var Devices = [
    {
        name:"客厅电视",
        commands:[
            {
                label:"开机",
                value:"4604,4444,646,488,644,1624,646,1622,646,1622,644,488,646,488,644,488,646,488,646,488,646,1622,646,1624,644,1622,646,488,644,490,644,488,644,490,644,490,644,490,644,1624,644,1622,644,490,644,488,644,490,646,488,646,1622,648,1618,646,486,648,486,648,1620,672,1596,648,1618,650,1620,646,46524,4606,4440,646,1620,646",
            },
        ],
    },
    {
        name:"客厅机顶盒",
        commands:[
        ],
    }
]
jQuery(document).ready(function(){
    var content = '<ul data-bind="foreach: Devices"> \
    <li> \
        The current item is: <b data-bind="text: name"></b> \
    </li> \
</ul>'
        jQuery('#content').html(content);
        ko.applyBindings({ Devices: Devices }, document.getElementById("txn-details"));

    
})