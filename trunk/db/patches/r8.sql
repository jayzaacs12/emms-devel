update tblConfiguration set eng = 'r8', esp = 'r8', fra = 'r8' where var = 'update';
create index loan_id on tblLoansMasterDetails (loan_id);
create index master_id on tblLoansMasterDetails (master_id);
create index date on tblLoanStatusHistory (date);
create index status on tblLoanStatusHistory (status);
create index p_status on tblLoanStatusHistory (p_status);
create index master_id on tblFundsLoansMasterPct (master_id);
create index delivered_date on tblLoans (delivered_date);