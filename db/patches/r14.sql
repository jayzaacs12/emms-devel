update tblConfiguration set eng = 'r14', esp = 'r14', fra = 'r14' where var = 'update';

DROP TABLE IF EXISTS `tblLoansTrash`;
CREATE TABLE  `tblLoansTrash` (
  `id` int(11) NOT NULL,
  `loan_code` tinytext NOT NULL,
  `status` enum('N','R','O','S','A','D','G','C','LI','LO','RT','RO') default NULL,
  `client_id` int(11) NOT NULL default '0',
  `loan_type_id` tinyint(4) NOT NULL default '0',
  `business_id` int(11) NOT NULL default '0',
  `installment` int(11) NOT NULL default '0',
  `fees_at` float(5,2) NOT NULL default '0.00',
  `fees_af` float(5,2) NOT NULL default '0.00',
  `rates_r` decimal(4,2) NOT NULL default '0.00',
  `rates_d` decimal(4,2) NOT NULL default '0.00',
  `rates_e` decimal(5,2) NOT NULL default '0.00',
  `margin_d` tinyint(4) NOT NULL default '0',
  `kp` decimal(10,2) NOT NULL default '0.00',
  `kat` decimal(10,2) NOT NULL default '0.00',
  `kaf` decimal(10,2) NOT NULL default '0.00',
  `pmt` decimal(10,2) NOT NULL default '0.00',
  `savings_p` decimal(10,2) NOT NULL default '0.00',
  `savings_v` decimal(10,2) NOT NULL default '0.00',
  `pg_value` decimal(10,2) NOT NULL default '0.00',
  `pg_memo` tinytext NOT NULL,
  `re_value` decimal(10,2) NOT NULL default '0.00',
  `re_memo` tinytext NOT NULL,
  `fgd_value` decimal(10,2) NOT NULL default '0.00',
  `fgd_memo` tinytext NOT NULL,
  `fgt_value` decimal(10,2) NOT NULL default '0.00',
  `fgt_memo` tinytext NOT NULL,
  `zone_id` int(11) NOT NULL default '0',
  `client_zone_id` int(11) NOT NULL default '0',
  `program_id` int(11) NOT NULL default '0',
  `advisor_id` int(11) NOT NULL default '0',
  `creator_date` date default NULL,
  `creator_id` int(11) default NULL,
  `editor_date` date default NULL,
  `editor_id` int(11) NOT NULL default '0',
  `delivered_date` date NOT NULL default '0000-00-00',
  `first_payment_date` date NOT NULL default '0000-00-00',
  `xp_cancel_date` date NOT NULL,
  `xp_num_pmt` int(11) NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `client_id` (`client_id`),
  KEY `delivered_date` (`delivered_date`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoanStatusHistoryTrash`;
CREATE TABLE  `tblLoanStatusHistoryTrash` (
  `id` int(11) NOT NULL,
  `loan_id` int(11) NOT NULL default '0',
  `p_status` varchar(16) NOT NULL default '',
  `status` varchar(16) NOT NULL default '',
  `date` date NOT NULL default '0000-00-00',
  `time` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `user_id` int(11) NOT NULL default '0',
  `memo` text NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `loan_id` (`loan_id`),
  KEY `date` (`date`),
  KEY `status` (`status`),
  KEY `p_status` (`p_status`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='LOANS';

DROP TABLE IF EXISTS `tblLoansMasterDetailsTrash`;
CREATE TABLE  `tblLoansMasterDetailsTrash` (
  `master_id` int(11) NOT NULL default '0',
  `loan_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`master_id`,`loan_id`),
  KEY `loan_id` (`loan_id`),
  KEY `master_id` (`master_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci COMMENT='LOANS';


DROP TABLE IF EXISTS `tblLoansMasterTrash`;
CREATE TABLE  `tblLoansMasterTrash` (
  `id` int(11) NOT NULL,
  `borrower_id` int(11) NOT NULL default '0',
  `borrower_type` enum('B','G','I') collate latin1_general_ci NOT NULL default 'B',
  `loan_type_id` int(11) NOT NULL default '0',
  `amount` decimal(13,2) NOT NULL default '0.00',
  `check_number` varchar(128) collate latin1_general_ci NOT NULL default '',
  `check_status` enum('P','A','D','R') collate latin1_general_ci NOT NULL default 'P',
  `program_id` int(11) NOT NULL default '0',
  `zone_id` int(11) NOT NULL default '0',
  `creator_id` int(11) NOT NULL default '0',
  `creator_date` date NOT NULL default '0000-00-00',
  `editor_id` int(11) NOT NULL default '0',
  `editor_date` date NOT NULL default '0000-00-00',
  `xp_delivered_date` date NOT NULL,
  `xp_first_payment_date` date NOT NULL,
  `chk_process` int(11) NOT NULL default '0',
  `sponsor_id` int(11) default '0',
  `kiva_id` int(11) default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci COMMENT='LOANS';

DROP TABLE IF EXISTS `tblFundsLoansMasterPctTrash`;
CREATE TABLE  `tblFundsLoansMasterPctTrash` (
  `id` int(11) NOT NULL,
  `master_id` int(11) NOT NULL default '0',
  `fund_id` int(11) NOT NULL default '0',
  `pct` int(3) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `master_id` (`master_id`)
) ENGINE=MyISAM AUTO_INCREMENT=11826 DEFAULT CHARSET=latin1 COMMENT='LOANS';

DELIMITER $$
DROP TRIGGER IF EXISTS tblLoans_bd$$
CREATE TRIGGER tblLoans_bd BEFORE DELETE ON tblLoans FOR EACH ROW
BEGIN
  INSERT INTO tblLoansTrash (id,loan_code,status,client_id,loan_type_id,business_id,installment,fees_at,fees_af,rates_r,rates_d,rates_e,margin_d,kp,kat,kaf,pmt,savings_p,savings_v,pg_value,pg_memo,re_value,re_memo,fgd_value,fgd_memo,fgt_value,fgt_memo,zone_id,client_zone_id,program_id,advisor_id,creator_date,creator_id,editor_date,editor_id,delivered_date,first_payment_date,xp_cancel_date,xp_num_pmt) VALUES (OLD.id,OLD.loan_code,OLD.status,OLD.client_id,OLD.loan_type_id,OLD.business_id,OLD.installment,OLD.fees_at,OLD.fees_af,OLD.rates_r,OLD.rates_d,OLD.rates_e,OLD.margin_d,OLD.kp,OLD.kat,OLD.kaf,OLD.pmt,OLD.savings_p,OLD.savings_v,OLD.pg_value,OLD.pg_memo,OLD.re_value,OLD.re_memo,OLD.fgd_value,OLD.fgd_memo,OLD.fgt_value,OLD.fgt_memo,OLD.zone_id,OLD.client_zone_id,OLD.program_id,OLD.advisor_id,OLD.creator_date,OLD.creator_id,OLD.editor_date,OLD.editor_id,OLD.delivered_date,OLD.first_payment_date,OLD.xp_cancel_date,OLD.xp_num_pmt);
END$$

﻿DELIMITER $$
DROP TRIGGER IF EXISTS tblLoansMaster_bd$$
CREATE TRIGGER tblLoansMaster_bd BEFORE DELETE ON tblLoansMaster FOR EACH ROW
BEGIN
  INSERT INTO tblLoansMasterTrash (id,borrower_id,borrower_type,loan_type_id,amount,check_number,check_status,program_id,zone_id,creator_id,creator_date,editor_id,editor_date,xp_delivered_date,xp_first_payment_date,chk_process,sponsor_id,kiva_id) VALUES (OLD.id,OLD.borrower_id,OLD.borrower_type,OLD.loan_type_id,OLD.amount,OLD.check_number,OLD.check_status,OLD.program_id,OLD.zone_id,OLD.creator_id,OLD.creator_date,OLD.editor_id,OLD.editor_date,OLD.xp_delivered_date,OLD.xp_first_payment_date,OLD.chk_process,OLD.sponsor_id,OLD.kiva_id);
END$$

﻿DELIMITER $$
DROP TRIGGER IF EXISTS tblLoansMasterDetails_bd$$
CREATE TRIGGER tblLoansMasterDetails_bd BEFORE DELETE ON tblLoansMasterDetails FOR EACH ROW
BEGIN
  INSERT INTO tblLoansMasterDetailsTrash (master_id,loan_id) VALUES (OLD.master_id,OLD.loan_id);
END$$

﻿DELIMITER $$
DROP TRIGGER IF EXISTS tblLoanStatusHistory_bd$$
CREATE TRIGGER tblLoanStatusHistory_bd BEFORE DELETE ON tblLoanStatusHistory FOR EACH ROW
BEGIN
  INSERT INTO tblLoanStatusHistoryTrash (id,loan_id,p_status,status,date,time,user_id,memo) VALUES (OLD.id,OLD.loan_id,OLD.p_status,OLD.status,OLD.date,OLD.time,OLD.user_id,OLD.memo);
END$$

﻿DELIMITER $$
DROP TRIGGER IF EXISTS tblFundsLoansMasterPct_bd$$
CREATE TRIGGER tblFundsLoansMasterPct_bd BEFORE DELETE ON tblFundsLoansMasterPct FOR EACH ROW
BEGIN
  INSERT INTO tblFundsLoansMasterPctTrash (id,master_id,fund_id,pct) VALUES (OLD.id,OLD.master_id,OLD.fund_id,OLD.pct);
END$$
