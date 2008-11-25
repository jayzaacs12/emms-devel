<script type="text/javascript" language="javascript">
    function makeRequest(url) {
        var httpRequest;
        if (window.XMLHttpRequest) { // Mozilla, Safari, ...
            httpRequest = new XMLHttpRequest();
            if (httpRequest.overrideMimeType) {
                httpRequest.overrideMimeType('text/xml');
                // See note below about this line
            }
        }
        else if (window.ActiveXObject) { // IE
            try {
                httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
            }
            catch (e) {
                try {
                    httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
                }
                catch (e) {}
            }
        }
        if (!httpRequest) {
            alert('Giving up :( Cannot create an XMLHTTP instance');
            return false;
        }
        httpRequest.onreadystatechange = function() { alertContents(httpRequest); };
        httpRequest.open('GET', url, true);
        httpRequest.send('');
    }

    function alertContents(httpRequest) {
        if (httpRequest.readyState == 4) {
            if (httpRequest.status == 200) {
                // alert(httpRequest.responseText);
                print();
            } else {
                alert('There was a problem with the request.');
            }
        }
    }
</script>

<h1>{page_title}</h1>{date}
{message}
<a href="index.popup.php?scr_name=LN.SCR.browseLoanMasterDuplicates&id={master_id}">{duplicates}</a>
<br><br>
<table cellpadding=0 cellspacing=0>
  <tr><td class=label>{zone_label}			</td><td>{zone}</td></tr>
  <tr><td class=label>{borrower_label}		</td><td>{borrower}</td></tr>
  <tr><td class=label>{borrower_type_label}	</td><td>{borrower_type}</td></tr>
  <tr><td class=label>{loan_type_label}		</td><td>{loan_type}</td></tr>
  <tr><td class=label>{amount_label}		</td><td>{amount}</td></tr>
  <tr><td class=label>{check_number_label}	</td><td>{check_number}</td></tr>
  <tr><td class=label>{check_status_label}	</td><td>{check_status}</td></tr>
  <tr><td class=label>{xp_delivered_date_label}	</td><td>{xp_delivered_date}</td></tr>
  <tr><td class=label><br>{chart_title}	</td><td></td></tr>
</table>
{chart}
{print}