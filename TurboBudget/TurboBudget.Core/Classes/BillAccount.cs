using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using TurboBudget.Types;

namespace TurboBudget.Core
{
    public class BillAccount : INamed, IAccount, IBillAccount, IMustAccount
    {
        #region INamed Properties

        string INamed.Name { get; set; }
        string INamed.Description { get; set; }

        #endregion
        #region IBillAccount Properties

        string IBillAccount.AccountNumber { get; set; }

        #endregion
        #region IMustAccount Properties

        decimal IMustAccount.MinimumAmount { get; set; }

        #endregion
    }
}
