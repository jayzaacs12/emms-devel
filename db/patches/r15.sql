CREATE OR REPLACE VIEW view_loan_status_max_id AS
  select
    max(id) id, loan_id
  from
    tblLoanStatusHistory
  group by
    loan_id;

CREATE OR REPLACE VIEW view_loan_status AS
  select
    lsm.loan_id, lsh.status
  from
    tblLoanStatusHistory lsh, view_loan_status_max_id lsm
  where
    lsh.id = lsm.id;

CREATE OR REPLACE VIEW view_sponsor_balance AS
  select
    lm.sponsor_id,sum(lcd.balance_kp) balance
  from
    tblLoansCurrentData lcd, tblLoansMaster lm, tblLoansMasterDetails lmd
  where
    lmd.loan_id = lcd.loan_id and
    lm.id = lmd.master_id
  group by
    lm.sponsor_id;

CREATE OR REPLACE VIEW view_sponsor_write_off AS
  select
    lm.sponsor_id,sum(lwo.principal) write_off
  from
    tblLoanWriteOff lwo, tblLoansMaster lm, tblLoansMasterDetails lmd
  where
    lmd.loan_id = lwo.loan_id and
    lm.id = lmd.master_id
  group by
    lm.sponsor_id;

ALTER TABLE tblSponsorsDonations ADD COLUMN new_clients ENUM('0','1') NOT NULL DEFAULT 0 AFTER sponsor_id;

update tblConfiguration set eng = 'r15', esp = 'r15', fra = 'r15' where var = 'update';