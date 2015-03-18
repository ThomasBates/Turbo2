using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Turbo.Budget.Types;

namespace Turbo.Budget.MockRepository
{
    class TurboBudgetMockAccount : ITurboBudgetAccount
    {
        public TurboBudgetMockAccount()
        {
        }

        public TurboBudgetMockAccount(int id,
                                      string name,
                                      string caption,
                                      string description)
        {
            ((ITurboBudgetAccount)this).ID = id;
            ((ITurboBudgetAccount)this).Name = name;
            ((ITurboBudgetAccount)this).Caption = caption;
            ((ITurboBudgetAccount)this).Description = description;
        }

        int ITurboBudgetAccount.ID { get; set; }
        string ITurboBudgetAccount.Name { get; set; }
        string ITurboBudgetAccount.Caption { get; set; }
        string ITurboBudgetAccount.Description { get; set; }
    }
}
